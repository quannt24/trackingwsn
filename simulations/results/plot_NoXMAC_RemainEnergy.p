set title "Remaining energy (without XT-MAC)"
set xlabel "Row (corresponding to y coordinate)"
set ylabel "Column (corresponding to x coordinate)"
set cbrange [0:3]
set cblabel "Energy (J)"
set xrange [-0.5:19.5]
set yrange [-0.5:19.5]
plot "NoXMAC_RemainEnergy.data" using 1:2:5 with image
