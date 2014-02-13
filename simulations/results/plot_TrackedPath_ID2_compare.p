set title "Tracked path of target ID2"
plot "../path_3.txt" u 1:2 t "True path" w lines, \
     "Default_TrackedPath_ID2.data" u 1:2 t "Tracked path (with X-MAC)" w lines, \
     "NoXMAC_TrackedPath_ID2.data" u 1:2 t "Tracked path (without X-MAC)" w lines
