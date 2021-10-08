#include "../types/types.h"

#define WORD(hi, lo) ( ((uint8)(hi) << 8 ) + (uint8)(lo) )

void outb(uint16 port, uint8 value) {
    __asm__ volatile (
        "movw %h0, %%dx\n\t"
        "outb %%al, %%dx\n\t"
        :: "irm" (port), "a" (value)
    );
}

uint8 inb(uint16 port) {
    uint8 value;
    __asm__ volatile (
        "movw %h1, %%dx\n\t"
        "inb %%dx, %%al\n\t"
        : "=a" (value)
        : "irm" (port)
    );
    return value;
}
