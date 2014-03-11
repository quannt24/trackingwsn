set title "Total energy"
set xlabel "Time (s)"
set ylabel "Total energy (J)"
set yrange [600:1300]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 10
set output 'TotalEnergy.eps'

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 5   # square
set style line 3 lc rgb '#0000ff' pt 9   # triangle

plot "Default - TotalEnergy.csv" u 1:2 t "with XT-MAC" w lines ls 1, \
     "BMAC - TotalEnergy.csv" u 1:2 t "with B-MAC" w lines ls 2
