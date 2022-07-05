
KERNEL_START = 0x10000;
KERNEL_ENTRYPOINT = 0x11000;
MEMORY_FREE_REAL_MEM_STACK = 0x100000;
MEMORY_PAGE_STRUCTURES = 0x500000;

SECTIONS {
    .text KERNEL_ENTRYPOINT : {
        * (.text)
    }
    .rodata : {
        * (.rodata)
    }
    .data : {
        * (.data)
    }
    .bss : {
        * (.bss)
    }
    KERNEL_SIZE = ALIGN(0x1000) - KERNEL_START;
}
