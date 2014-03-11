set title "End-to-end Delay"
set xlabel "Time (s)"
set ylabel "Delay (s)"
set xrange [0:150]
set yrange [0:2.5]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 10
set output 'E2eDelay.eps'

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 5   # square
set style line 3 lc rgb '#0000ff' pt 9   # triangle

plot "Default - E2eDelay.csv" u 1:2 t "With XT-MAC" ls 1, \
     "BMAC - E2eDelay.csv" u 1:2 t "With B-MAC" ls 3
