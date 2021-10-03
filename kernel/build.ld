OUTPUT_FORMAT("binary")
SECTIONS {
    .text 0 : {
        * (.text)
    }
    .prot 0x10000 : {
        * (.prot)
    }
}
