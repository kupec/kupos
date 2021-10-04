#include "console.h"
#include "string.h"

#define TEXT_MODE_WIDTH 80
#define VIDEO_BUFFER ((char *)0xb8000)

void video_set_cursor(int row, int col) {
    int offset = (row - 1) * TEXT_MODE_WIDTH + (col - 1);

    __asm__ volatile (
        "movw $0x3d4, %%dx\n\t"
        "movb $14, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "movl %0, %%eax\n\t"
        "movb %%ah, %%al\n\t"
        "incw %%dx\n\t"
        "outb %%al, %%dx\n\t"
        "decw %%dx\n\t"
        "movb $15, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "movl %0, %%eax\n\t"
        "incw %%dx\n\t"
        "outb %%al, %%dx\n\t"
        :: "r" (offset)
        : "eax", "edx"
    );
}

void video_get_cursor(int *row, int *col) {
    int offset;

    __asm__ volatile (
        "xorl %%eax, %%eax\n\t"
        "movw $0x3d4, %%dx\n\t"
        "movb $14, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "incw %%dx\n\t"
        "inb %%dx, %%al\n\t"
        "movb %%al, %%ah\n\t"
        "decw %%dx\n\t"
        "movb $15, %%al\n\t"
        "outb %%al, %%dx\n\t"
        "incw %%dx\n\t"
        "inb %%dx, %%al\n\t"
        "movl %%eax, %0\n\t"
        : "=r" (offset)
        :: "eax", "edx"
    );

    *row = 1 + offset / TEXT_MODE_WIDTH;
    *col = 1 + offset % TEXT_MODE_WIDTH;
}

void puts(char* s) {
    int row, col;
    video_get_cursor(&row, &col);

    int len = strlen(s);
    int offset = (row - 1) * TEXT_MODE_WIDTH + (col - 1);

    char* video = VIDEO_BUFFER;
    for (int i = 0; i < len; i++) {
        video[(offset + i) * 2] = s[i];
    }

    offset += len;
    row = 1 + offset / TEXT_MODE_WIDTH;
    col = 1 + offset % TEXT_MODE_WIDTH;

    row++;
    col = 1;

    video_set_cursor(row, col);
}
