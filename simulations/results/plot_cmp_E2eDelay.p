set title "End-to-end Delay"
set xlabel "Time (s)"
set ylabel "Delay (s)"
set xrange [0:150]
plot "Default - E2eDelay.csv" u 1:2 t "With XT-MAC", \
     "BMAC - E2eDelay.csv" u 1:2 t "With B-MAC"
