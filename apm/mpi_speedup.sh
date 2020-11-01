BIN="./apmMPI"                               # Name of the binary file
PATTERNS="AAT TGA TTTTT 		"      # List of patters to test
FILE="dna/1mo_chrY.fa"                           # DNA test-file
APPROXIMATION=0                              # Approximation number

t_seq=$(mpirun -np 1 -f hosts $BIN $APPROXIMATION $FILE $(echo $PATTERNS) | grep APM | awk '{print $5}')
export OMP_NB_THREADS=1
for i in $(seq 1 12); do
	t_parallel=$(mpirun -np $i -f hosts $BIN $APPROXIMATION $FILE $(echo $PATTERNS) | grep APM | awk '{print $5}')
	#t_parallel=$(mpirun -np $i $BIN $APPROXIMATION $FILE $(echo $PATTERNS) | grep APM | awk '{print $5}')
        echo -e "$i\t$t_seq\t$t_parallel"
done
