OUTPUT_FORMAT("binary")
ENTRY("0")

KERNEL_START = 0x10000;
KERNEL_ENTRYPOINT = 0x11000;

SECTIONS {
    .text KERNEL_ENTRYPOINT : {
        * (.text)
        * (.rodata)
        * (.data)
        * (.bss)
    }
    KERNEL_SIZE = ALIGN(0x1000) - KERNEL_START;
}
