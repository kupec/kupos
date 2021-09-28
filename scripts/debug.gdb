target remote 192.168.0.70:1234
set architecture i8086
display/4i $cs*16+$pc
break *0x7c00
break *0x80f4
