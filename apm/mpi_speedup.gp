set terminal png 
set ylabel "speedup" 
set xlabel "number of processes" 
set xtics 1 
set title "APM Speedup for MPI execution" 
set output "SpeedUpAPM-MPI.png" 

plot x title 'Speedup max' with lines, 'mpi_speedup.data' using 1:($2/$3) title 'APM' with linespoints;
