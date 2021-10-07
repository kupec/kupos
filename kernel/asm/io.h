#include "../types/types.h"

#define outb(PORT, VALUE) \
    __asm__ volatile ( \
        "movw %h0, %%dx\n\t" \
        "outb %%al, %%dx\n\t" \
        :: "irm" (PORT), "a" ((uint8)(VALUE)) \
    )
#define inb(PORT, VALUE) \
    __asm__ volatile ( \
        "movw %h1, %%dx\n\t" \
        "inb %%dx, %%al\n\t" \
        : "=a" (VALUE) \
        : "irm" (PORT) \
    )
