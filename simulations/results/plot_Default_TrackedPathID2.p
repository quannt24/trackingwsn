set title "Tracked path"
set xlabel "x (m)"
set ylabel "y (m)"
set xrange [150:250]
set yrange [0:100]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 10
set output 'Default_TrackedPath.eps'

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 5   # square
set style line 3 lc rgb '#0000ff' pt 9   # triangle

plot "../path_3.txt" u 1:2 t "True path" w lines ls 1, \
     "Default_TrackedPath_ID2.data" u 1:2 t "Tracked path" w lines ls 2
