target remote 192.168.0.70:1234

define real_mode
set tdesc filename scripts/target.xml
display/4i $cs*16+$pc
end

define prot_mode
display/4i $pc
break *0x11000
end

symbol-file boot/mbr.elf -o 0x200
define n
next
list +3
end

break *0x7c00
