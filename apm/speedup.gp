set terminal png 
set ylabel "speedup" 
set xlabel "number of cores" 
set xtics 1 
set title "APM Speedup for OpenMP execution :" 
set output "SpeedUpAPM-OMP.png" 

plot x title 'Speedup max' with lines, \
 'static.data' using 1:($2/$3) title 'APM - Static' with linespoints, \
 'dynamic.data' using 1:($2/$3) title 'APM - Dynamic' with linespoints;
