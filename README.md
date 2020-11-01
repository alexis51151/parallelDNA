# APM parallelized for DNA analysis
DNA sequence comparison tool implementing Approximate Pattern Matching (APM) algorithm with levenshtein's distance.

## Libraries used

In this project, we intend to use primarly OpenMP and MPI in order to use parallelism for perfomance-enhancement compared to the sequential APM.
Might use CUDA at some moments if necessary.

## How to use

If you just want to compare performances between the sequential and parallel implementations, you might want to use our test script:
```bash
# Basic script that test 3 patterns on a one-line DNA file with approximation distance of 0
./test.sh
```
If you want to change the patterns, the approximation distance or the test file, just modify those lines in test.sh:
```bash
PATTERNS="CAG GTACAT GGG"   # List of patterns to test
FILE="dna/line_chrY.fa"     # DNA file
APPROXIMATION=0             # Approximation distance
```

Or if you want to execute one implementation, follow those steps  :
```bash
# Generate the binaries
make
```

- Sequential execution :
```bash
./apm approximation_factor dna_database pattern1 pattern2 ...
```
- Parallel execution :

With OpenMP:
```bash
export OMP_NUM_THREAD=number_of_cores;
./apmOMP approximation_factor dna_database pattern1 pattern2 ...
```
With MPI:
```bash
mpirun -np number_of_machines -f hosts ./apmMPI approximation_factor dna_database pattern1 pattern2 ...
```

Hybrid : 

```bash
export OMP_NUM_THREAD=number_of_cores;
mpirun -np number_of_machines -f hosts ./apmParallel approximation_factor dna_database pattern1 pattern2 ...
```


## Benchmarking

- OpenMP Benchmark :

```bash
./run.sh | tee run.data
gnuplot speedup.sh
display SpeedUpAPM-OMP.png
```

- MPI Benchmark :

```bash
./mpi_speedup.sh | tee mpi_speedup.data
gnuplot mpi_speedup.sh
display SpeedUpAPM-MPI.png
```

Detailed benchmark to be found in ```apm/results/Slides CSC5001.pdf```


## Authors
- Alexis Le Glaunec
- Alice Zhen
