set title "Tracked path"
set xlabel "x (m)"
set ylabel "y (m)"
set xrange [150:250]
set yrange [0:100]

set terminal postscript eps enhanced color font 'Helvetica,20' lw 8
set output 'TrackedPath.eps'

plot "../path_3.txt" u 1:2 t "True path" w lines, \
     "Default_TrackedPath_ID2.data" u 1:2 t "Tracked path (with XT-MAC)" w lines, \
     "BMAC_TrackedPath_ID2.data" u 1:2 t "Tracked path (with B-MAC)" w lines
