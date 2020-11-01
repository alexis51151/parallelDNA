set terminal png 
set ylabel "speedup" 
set xlabel "number of cores" 
set xtics 1 
set title "APM Speedup for Hybrid execution :" 
set output "SpeedUpAPM-OMP.png" 

plot x title 'Speedup max' with lines, \
 'run.data' using 1:($2/$3) title 'APM' with linespoints;
