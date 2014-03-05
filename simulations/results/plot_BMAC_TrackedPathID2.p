set title "Tracked path of target"
set xlabel "x (m)"
set ylabel "y (m)"
set xrange [-50:450]
set yrange [-50:450]
plot "../path_3.txt" u 1:2 t "True path" w lines, \
     "BMAC_TrackedPath_ID2.data" u 1:2 t "Tracked path" w lines
