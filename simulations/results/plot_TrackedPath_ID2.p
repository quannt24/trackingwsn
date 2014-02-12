set title "Tracked path of target ID2"
plot "../path_3.txt" u 1:2 t "True path" w lines, \
     "Default_TrackedPath_ID2.data" using 1:2 t "Tracked path (EMRP)" w lines, \
     "Arpees_TrackedPath_ID2.data" using 1:2 t "Tracked path (ARPEES)" w lines
