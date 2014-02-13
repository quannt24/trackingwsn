set title "Remaining energy"
set xrange [-0.5:19.5]
set yrange [-0.5:19.5]
plot "NoSleep_RemainEnergy.data" using 1:2:5 with image
