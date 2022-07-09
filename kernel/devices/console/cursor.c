#include "cursor.h"
#include "video.h"
#include "../../asm/io.h"

uint16 cursor_row;
uint16 cursor_col;

void console_read_cursor(uint16 *row, uint16 *col) {
    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_HI);
    uint8 offset_hi = inb(CRT_VAL_PORT);
    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_LO);
    uint16 offset = WORD(offset_hi, inb(CRT_VAL_PORT));

    offset -= video_get_page() * PAGE_SIZE;

    *row = offset / PAGE_WIDTH;
    *col = offset % PAGE_WIDTH;
}

void console_write_cursor(uint16 row, uint16 col) {
    uint16 offset = video_get_page() * PAGE_SIZE + row * PAGE_WIDTH + col;

    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_HI);
    outb(CRT_VAL_PORT, offset >> 8);
    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_LO);
    outb(CRT_VAL_PORT, offset);
}

void console_cursor_init() {
    console_read_cursor(&cursor_row, &cursor_col);
}

void console_cursor_set(uint16 row, uint16 col) {
    if (cursor_row == row && cursor_col == col) {
        return;
    }

    console_write_cursor(row, col);
    cursor_row = row;
    cursor_col = col;
}

void console_cursor_get(uint16 *row, uint16 *col) {
    *row = cursor_row;
    *col = cursor_col;
}
