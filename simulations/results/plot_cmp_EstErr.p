set title "Estimation Error"
set xlabel "Time (s)"
set ylabel "Error (m)"
set xrange [0:150]
plot "Default - EstErr.csv" u 1:2 t "With XT-MAC", \
     "BMAC - EstErr.csv" u 1:2 t "With B-MAC"
