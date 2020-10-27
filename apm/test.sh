#! /bin/zsh


BIN="apm" 				# Name of the binary file
PATTERNS="CAG GTACAT GGG" 	# List of patters to test
FILE="dna/1mo_chrY.fa" 		# DNA test-file
APPROXIMATION=0			# Approximation number
MPI_NP=4


 # Fresh generation of binaries
make clean > /dev/null;
make > /dev/null;


for algo in $(echo $BIN); do
	echo "---" ${algo} algorithm results  on $FILE "---"
	./${algo} $APPROXIMATION $FILE $(echo $PATTERNS)
done

# Test MPI implementation
echo "---" ${algo} parallelized algorithm results  on $FILE "---"
mpirun -np $MPI_NP ./apmMPI $APPROXIMATION $FILE $(echo $PATTERNS)

# Test OMP MPI implementation
echo "---" ${algo} parallelized algorithm results  on $FILE "---"
mpirun -np $MPI_NP ./apmOMPMPI $APPROXIMATION $FILE $(echo $PATTERNS)
