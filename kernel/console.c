#include "console.h"
#include "string.h"
#include "math.h"
#include "asm/io.h"

#define PAGE_WIDTH 80
#define PAGE_HEIGHT 25
#define PAGE_SIZE (PAGE_WIDTH * PAGE_HEIGHT)
#define VIDEO_BUFFER 0xb8000

#define CRT_REG_PORT 0x3d4
#define CRT_VAL_PORT 0x3d5
#define CRT_MEM_START_REG_HI 12
#define CRT_MEM_START_REG_LO 13
#define CRT_CURSOR_POS_REG_HI 14
#define CRT_CURSOR_POS_REG_LO 15

uint current_page = 0;

void console_set_cursor(uint16 row, uint16 col) {
    uint16 offset = current_page * PAGE_SIZE + row * PAGE_WIDTH + col;

    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_HI);
    outb(CRT_VAL_PORT, offset >> 8);
    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_LO);
    outb(CRT_VAL_PORT, offset);
}

void console_get_cursor(uint16 *row, uint16 *col) {
    uint16 offset = 0;
    uint16 offset_hi = 0;

    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_HI);
    inb(CRT_VAL_PORT, offset_hi);
    outb(CRT_REG_PORT, CRT_CURSOR_POS_REG_LO);
    inb(CRT_VAL_PORT, offset);

    offset += offset_hi << 8;
    offset -= current_page * PAGE_SIZE;

    *row = offset / PAGE_WIDTH;
    *col = offset % PAGE_WIDTH;
}

void console_select_page(uint page) {
    uint16 offset = page * PAGE_SIZE;

    outb(CRT_REG_PORT, CRT_MEM_START_REG_HI);
    outb(CRT_VAL_PORT, offset >> 8);
    outb(CRT_REG_PORT, CRT_MEM_START_REG_LO);
    outb(CRT_VAL_PORT, offset);

    current_page = page;
}

uint16 *get_video_page_ptr(uint page) {
    return VIDEO_BUFFER + page * PAGE_SIZE * 2;
}

void console_print_line(char* s) {
    uint16 row, col;
    console_get_cursor(&row, &col);

    int scroll_line_count = max(0, row + 2 - PAGE_HEIGHT);
    int start_pos = scroll_line_count * PAGE_WIDTH;
    int end_pos = row * PAGE_WIDTH;
    row -= scroll_line_count;

    uint next_page = 1 - current_page;
    uint16* current_buffer = get_video_page_ptr(current_page);
    uint16* next_buffer = get_video_page_ptr(next_page);

    for (int i = 0; i < PAGE_SIZE; i++) {
        next_buffer[i] = 0x0720;
    }
    for (int i = start_pos; i < end_pos; i++) {
        next_buffer[i - start_pos] = current_buffer[i];
    } 

    int len = min(strlen(s), PAGE_WIDTH - col);
    int cursor = row * PAGE_WIDTH + col;

    for (int i = 0; i < len; i++) {
        ((char*)next_buffer)[(cursor + i) * 2] = s[i];
    }

    console_select_page(next_page);
    console_set_cursor(row + 1, 0);
}
