#! /bin/zsh


BIN="apm apmOMP" 			# Name of the binary file
PATTERNS="CAG GTACAT GGG" 	# List of patters to test
FILE="dna/small_chrY.fa" 	# DNA test-file
APPROXIMATION=0			# Approximation number


 # Fresh generation of binaries
make clean > /dev/null;
make > /dev/null;


for algo in $(echo $BIN); do
	./${algo} $APPROXIMATION $FILE $(echo $PATTERNS)
done
