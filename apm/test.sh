#! /bin/zsh


BIN="apm" # Name of the binary file
PATTERNS="CAG GTACAT GGG" # List of patters to test
FILE="dna/line_chrY.fa" # Test file
APPROXIMATION=0


if [ ! -f $BIN ]; then
	make clean; make; # If no binary, make it
fi

./$BIN $APPROXIMATION $FILE $(echo $PATTERNS)
