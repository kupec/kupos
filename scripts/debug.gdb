target remote 192.168.0.70:1234
#set architecture i8086
#display/4i $cs*16+$pc
display/4i $pc
break *0x11000
