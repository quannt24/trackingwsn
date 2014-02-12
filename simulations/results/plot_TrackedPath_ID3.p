set title "Tracked path of target ID3"
plot "../path_4.txt" u 1:2 t "True path" w lines, \
     "Default_TrackedPath_ID3.data" using 1:2 t "Tracked path" w lines
