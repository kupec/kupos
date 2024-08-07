#pragma once

#include "../types/types.h"

#define WORD(hi, lo) ( ((uint8)(hi) << 8 ) + (uint8)(lo) )

static void outb(uint16 port, uint8 value) {
    __asm__ volatile (
        "movw %h0, %%dx\n\t"
        "outb %%al, %%dx\n\t"
        :: "irm" (port), "a" (value)
        : "edx"
    );
}

static void outl(uint16 port, uint32 value) {
    __asm__ volatile (
        "movw %h0, %%dx\n\t"
        "outl %%eax, %%dx\n\t"
        :: "irm" (port), "a" (value)
        : "edx"
    );
}

static uint8 inb(uint16 port) {
    uint8 value;
    __asm__ volatile (
        "movw %h1, %%dx\n\t"
        "inb %%dx, %%al\n\t"
        : "=a" (value)
        : "irm" (port)
        : "edx"
    );
    return value;
}

static uint32 inl(uint16 port) {
    uint32 value;
    __asm__ volatile (
        "movw %h1, %%dx\n\t"
        "inl %%dx, %%eax\n\t"
        : "=a" (value)
        : "irm" (port)
        : "edx"
    );
    return value;
}
