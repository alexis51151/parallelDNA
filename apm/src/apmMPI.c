/**
 * APPROXIMATE PATTERN MATCHING
 *
 * INF560 X2016
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <mpi.h>

#define APM_DEBUG 0

char *
read_input_file( char * filename, int * size )
{
    char * buf ;
    off_t fsize;
    int fd = 0 ;
    int n_bytes = 1 ;

    /* Open the text file */
    fd = open( filename, O_RDONLY ) ;
    if ( fd == -1 )
    {
        fprintf( stderr, "Unable to open the text file <%s>\n", filename ) ;
        return NULL ;
    }


    /* Get the number of characters in the textfile */
    fsize = lseek(fd, 0, SEEK_END);
    if(fsize == (off_t) -1){
	    fprintf( stderr, "Unable to find the file size <%s>\n", filename ) ;
	    return NULL ;
    }
    if (lseek(fd, 0, SEEK_SET) < 0) {
	    fprintf(stderr, "Unable to lseek to the beginning");
	    return NULL;
    }

#if APM_DEBUG
    printf( "File length: %lld\n", fsize ) ;
#endif

    /* Allocate data to copy the target text */
    buf = (char *)malloc( fsize * sizeof ( char ) ) ;
    if ( buf == NULL )
    {
        fprintf( stderr, "Unable to allocate %ld byte(s) for main array\n",
                fsize ) ;
        return NULL ;
    }
    n_bytes = read( fd, buf, fsize ) ;
    if ( n_bytes != fsize )
    {
        fprintf( stderr,
                "Unable to copy %ld byte(s) from text file (%d byte(s) copied)\n",
                fsize, n_bytes) ;
        return NULL ;
    }

#if APM_DEBUG
    printf( "Number of read bytes: %d\n", n_bytes ) ;
#endif

    *size = n_bytes ;


    close( fd ) ;


    return buf ;
}


#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

int levenshtein(char *s1, char *s2, int len, int * column) {
    unsigned int x, y, lastdiag, olddiag;

    for (y = 1; y <= len; y++)
    {
        column[y] = y;
    }
    for (x = 1; x <= len; x++) {
        column[0] = x;
        lastdiag = x-1 ;
        for (y = 1; y <= len; y++) {
            olddiag = column[y];
            column[y] = MIN3(
                    column[y] + 1,
                    column[y-1] + 1,
                    lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1)
                    );
            lastdiag = olddiag;

        }
    }
    return(column[len]);
}




int
main( int argc, char ** argv )
{
  MPI_Init(&argc, &argv);

  char ** pattern ;
  char * filename ;
  int approx_factor = 0 ;
  int nb_patterns = 0 ;
  int i, j ;
  char * buf ;
  struct timeval t1, t2;
  double duration ;
  int n_bytes = 0;
  int * n_matches ;

  int world;
  int rank;
  MPI_Comm_size(MPI_COMM_WORLD, &world);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
#if APM_DEBUG
  printf( "Starting process %d/%d\n", rank, world);
#endif

  /* Check number of arguments */
  if ( argc < 4 )
  {
    printf( "Usage: %s approximation_factor "
            "dna_database pattern1 pattern2 ...\n",
            argv[0] ) ;
    return 1 ;
  }

  /* Get the distance factor */
  approx_factor = atoi( argv[1] ) ;

  /* Grab the filename containing the target text */
  filename = argv[2] ;

  /* Get the number of patterns that the user wants to search for */
  nb_patterns = argc - 3 ;

  /* Fill the pattern array */
  pattern = (char **)malloc( nb_patterns * sizeof( char * ) ) ;
  if ( pattern == NULL )
  {
      fprintf( stderr,
              "Unable to allocate array of pattern of size %d\n",
              nb_patterns ) ;
      return 1 ;
  }

  /* Grab the patterns */
  for ( i = 0 ; i < nb_patterns ; i++ )
  {
      int l ;

      l = strlen(argv[i+3]) ;
      if ( l <= 0 )
      {
          fprintf( stderr, "Error while parsing argument %d\n", i+3 ) ;
          return 1 ;
      }

      pattern[i] = (char *)malloc( (l+1) * sizeof( char ) ) ;
      if ( pattern[i] == NULL )
      {
          fprintf( stderr, "Unable to allocate string of size %d\n", l ) ;
          return 1 ;
      }

      strncpy( pattern[i], argv[i+3], (l+1) ) ;

  }


  printf( "[%d]: Approximate Pattern Matching with MPI: "
          "looking for %d pattern(s) in file %s w/ distance of %d\n",
          rank, nb_patterns, filename, approx_factor ) ;


  /* Allocate the array of matches */
  n_matches = (int *)malloc( nb_patterns * sizeof( int ) ) ;
  if ( n_matches == NULL )
  {
      fprintf( stderr, "Error: unable to allocate memory for %ldB\n",
              nb_patterns * sizeof( int ) ) ;
      return 1 ;
  }

  /*****
   * BEGIN MAIN LOOP
   ******/

  /* Timer start */
  gettimeofday(&t1, NULL);


  /* The DNA file of size `n_bytes` should be divided into `rank` parts
     so that each MPI process has one part to take care of.
     Process `rank` will be responsible to look for every substring in the bytes
     from `rank * (n_bytes // size)` to `(rank + 1) * (n_bytes // size) - 1`
     The potential remaining bytes are handled by thread `world`.
  */

  /* Wait for processes to start */
  MPI_Barrier(MPI_COMM_WORLD);

  if (rank == 0) {
      /* Process 0 reads the input file and broadcasts the file size */
      buf = read_input_file( filename, &n_bytes ) ;
      if ( buf == NULL )
      {
          return 1 ;
      }
      MPI_Bcast(&n_bytes, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  } else {
      /* Other processes receive the broadcast */
      MPI_Bcast(&n_bytes, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
      /* They need to allocate data to copy the target text according to the received size*/
      buf = (char *)malloc( n_bytes * sizeof ( char ) ) ;
      if ( buf == NULL )
      {
          fprintf( stderr, "Unable to allocate %ld byte(s) for buf array\n",
                  n_bytes ) ;
          return EXIT_FAILURE;
      }
  }

  /* Broadcast `buf` and `n_bytes` from process 0 so every process can access
     the length and the content of the dna file */
  MPI_Bcast(buf, n_bytes + 1, MPI_BYTE, 0, MPI_COMM_WORLD);

  /* The last process should check the remaining bytes
     from `world * (n_bytes / world)` to `n_bytes` */
  int start = rank * (n_bytes / world);
  int end = (rank + 1) * (n_bytes / world) - 1;
  if (rank == world - 1) {
      end = n_bytes;
  }

#if APM_DEBUG
  printf( "[%d] will process bytes %d to %d\n", rank, start, end);
#endif


  for ( i = 0 ; i < nb_patterns ; i++ )
  {
      int size_pattern = strlen(pattern[i]) ;

      int * column = (int *)malloc( (size_pattern+1) * sizeof( int ) ) ;

      if ( column == NULL )
      {
          fprintf( stderr, "Error: unable to allocate memory for column (%ldB)\n",
                  (size_pattern+1) * sizeof( int ) ) ;
          return 1 ;
      }

      n_matches[i] = 0 ;

      for ( j = start ; j < end ; j++ )
      {
          int distance = 0 ;
          int size ;

#if APM_DEBUG
          if ( j % 100 == 0 )
          {
              printf( "[%d]: processing byte %d (out of %d) for pattern %s\n", rank, j, n_bytes, pattern[i] ) ;
          }
#endif

          /* Process `rank` should check every substring starting in its attributed range.
             The size is equals to the pattern size except for when the end of the buffer is reached.*/
          size = size_pattern ;
          if ( n_bytes - j < size_pattern )
          {
              size = n_bytes - j ;
          }

          distance = levenshtein( pattern[i], &buf[j], size, column ) ;

          if ( distance <= approx_factor ) {
              n_matches[i]++ ;
          }
      }
      free( column );
  }

#if APM_DEBUG
  for ( i = 0 ; i < nb_patterns ; i++ )
  {
      printf( "[%d]: Number of matches for pattern <%s>: %d\n", rank, pattern[i], n_matches[i] ) ;
  }
#endif

  /* Each process sends its `n_matches` array to process 0
  Process 0 receives each array and sums it to its own `n_matches`*/
  if (rank == 0) {


      /* Reveives `n_matches` for every process and sums it to its own `n_matches` */
      MPI_Status status;
      for (i=1; i<world; i++) {
          /* Allocate an array to receive matches */
          int * n_matches_recv = (int *)malloc( nb_patterns * sizeof( int ) ) ;
          if ( n_matches_recv == NULL )
          {
              fprintf( stderr, "Error: unable to allocate memory for %ldB\n",
                      nb_patterns * sizeof( int ) ) ;
              return 1 ;
          }

          MPI_Recv(n_matches_recv, nb_patterns, MPI_INTEGER, i, i, MPI_COMM_WORLD, &status);
          for (j=0; j<nb_patterns; j++) {
            n_matches[j] = n_matches[j] + n_matches_recv[j];
          }

          /* Free array after receiving*/
          free(n_matches_recv);
      }
  } else {
      MPI_Send(n_matches, nb_patterns, MPI_INTEGER, 0, rank, MPI_COMM_WORLD);
  }


  /* Timer stop */
  gettimeofday(&t2, NULL);

  duration = (t2.tv_sec -t1.tv_sec)+((t2.tv_usec-t1.tv_usec)/1e6);

  printf( "[%d]: APM done in %lf s\n", rank, duration ) ;

  /*****
   * END MAIN LOOP
   ******/

  if (rank == 0) {
      for ( i = 0 ; i < nb_patterns ; i++ )
      {
          printf( "Total number of matches for pattern <%s>: %d\n", pattern[i], n_matches[i] ) ;
      }
  }

  MPI_Finalize();

  return 0 ;
}
