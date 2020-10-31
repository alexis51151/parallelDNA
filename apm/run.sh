export GOMP_CPU_AFFINITY=`seq -s"," 0 47`



BIN="apm"                               # Name of the binary file
PATTERNS="CAG GTACAT GGG"       # List of patters to test
FILE="dna/1mo_chrY.fa"          # DNA test-file
APPROXIMATION=0                 # Approximation number
MPI_NP=2
export OMP_NUM_THREADS=1


t_seq=$(./apm $APPROXIMATION $FILE $(echo $PATTERNS) | grep APM | awk '{print $4}')
for i in $(seq 1 47); do
	export OMP_NUM_THREADS=$i
	t_parallel=$(mpirun -np $MPI_NP ./apmParallel $APPROXIMATION $FILE $(echo $PATTERNS) | grep APM | awk '{print $4}')
        echo -e "$i\t$t_seq\t$t_parallel"
done

