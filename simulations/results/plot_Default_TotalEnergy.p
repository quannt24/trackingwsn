set title "Total energy"
set xlabel "Time (s)"
set ylabel "Total energy (J)"
set yrange [0:1300]
plot "Default - TotalEnergy.csv" u 1:2 notitle w lines
