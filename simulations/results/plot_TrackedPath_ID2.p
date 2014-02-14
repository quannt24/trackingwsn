set title "Tracked path of target ID2"
set xlabel "x (m)"
set ylabel "y (m)"
plot "../path_3.txt" u 1:2 t "True path" w lines, \
     "Default_TrackedPath_ID2.data" u 1:2 t "Tracked path" w lines
