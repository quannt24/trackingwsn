set title "Estimation Error"
set xlabel "Time (s)"
set ylabel "Error (m)"
set xrange [0:150]
set yrange [0:20]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 10
set output 'Default_EstErr.eps'

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 5   # square
set style line 3 lc rgb '#0000ff' pt 9   # triangle

plot "Default - EstErr.csv" u 1:2 notitle ls 1
