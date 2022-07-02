target remote 192.168.0.70:1234

define real_mode
    break *0x7c00
    continue
    set tdesc filename scripts/target.xml
    display/4i $cs*16+$pc
end

define prot_mode
    break *0x11000
    continue
    symbol-file kernel/main.elf
    display/4i $pc
end

