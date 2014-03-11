set title "Remaining energy"
set xlabel "Row (corresponding to y coordinate)"
set ylabel "Column (corresponding to x coordinate)"

set terminal postscript eps enhanced color font 'Helvetica,20' lw 10
set output 'Default_RemainEnergy.eps'

set border linewidth 0.5
set pointsize 1.5
set style line 1 lc rgb '#ff0000' pt 7   # circle
set style line 2 lc rgb '#00ff00' pt 5   # square
set style line 3 lc rgb '#0000ff' pt 9   # triangle

set cbrange [0:3]
set cblabel "Energy (J)"
set xrange [-0.5:19.5]
set yrange [-0.5:19.5]

plot "Default_RemainEnergy.data" using 1:2:5 with image
