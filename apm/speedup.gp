set terminal png 
set ylabel "speedup" 
set xlabel "number of cores" 
set xtics 1 
set title "APM Speedup for OpenMP and MPI  execution" 
set output "SpeedUpAPM-OMPMPI.png" 

plot x title 'Speedup max' with lines, 'run.data' using 1:($2/$3) title 'APM' with linespoints;
