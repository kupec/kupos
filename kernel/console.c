#include "console.h"
#include "string.h"
#include "math.h"

#define TEXT_MODE_WIDTH 80
#define VIDEO_BUFFER ((char *)0xb8000)
#define CRT_PORT 0x3d4
#define CRT_CURSOR_POS_REG 14

void console_set_cursor(int row, int col) {
    int offset = row * TEXT_MODE_WIDTH + col;

    __asm__ volatile (
        "movw %[port], %%dx\n\t"
        "movb %[cur_reg], %%al\n\t"
        "outb %%al, %%dx\n\t"
        "movl %[offset], %%eax\n\t"
        "movb %%ah, %%al\n\t"
        "incw %%dx\n\t"
        "outb %%al, %%dx\n\t"
        "decw %%dx\n\t"
        "movb %[cur_reg] + 1, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "movl %[offset], %%eax\n\t"
        "incw %%dx\n\t"
        "outb %%al, %%dx\n\t"
        :: [port] "i" (CRT_PORT),
           [cur_reg] "i" (CRT_CURSOR_POS_REG),
           [offset] "r" (offset)
        : "eax", "edx"
    );
}

void console_get_cursor(int *row, int *col) {
    int offset;

    __asm__ volatile (
        "xorl %%eax, %%eax\n\t"
        "movw %[port], %%dx\n\t"
        "movb %[cur_reg], %%al\n\t"
        "outb %%al, %%dx\n\t"
        "incw %%dx\n\t"
        "inb %%dx, %%al\n\t"
        "movb %%al, %%ah\n\t"
        "decw %%dx\n\t"
        "movb %[cur_reg] + 1, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "incw %%dx\n\t"
        "inb %%dx, %%al\n\t"
        "movl %%eax, %[offset]\n\t"
        : [offset] "=r" (offset)
        : [port] "i" (CRT_PORT),
          [cur_reg] "i" (CRT_CURSOR_POS_REG)
        : "eax", "edx"
    );

    *row = offset / TEXT_MODE_WIDTH;
    *col = offset % TEXT_MODE_WIDTH;
}

void console_print_line(char* s) {
    int row, col;
    console_get_cursor(&row, &col);

    int len = max(strlen(s), TEXT_MODE_WIDTH - col);
    int cursor = row * TEXT_MODE_WIDTH + col;

    char* video = VIDEO_BUFFER;
    for (int i = 0; i < len; i++) {
        video[(cursor + i) * 2] = s[i];
    }

    console_set_cursor(row + 1, 0);
}
