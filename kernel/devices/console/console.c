#include "console.h"
#include "video.h"
#include "cursor.h"
#include "../../std/string.h"
#include "../../std/math.h"
#include "../../asm/io.h"

void console_init() {
    console_cursor_init();
}

uint16 *get_video_page_ptr(uint page) {
    return VIDEO_BUFFER + page * PAGE_SIZE;
}

void console_clear_screen(uint page) {
    uint16* buffer = get_video_page_ptr(page);
    for (int i = 0; i < PAGE_SIZE; i++) {
        buffer[i] = WORD(WHITE_ON_BLACK, ' ');
    }
}

void console_draw_text(uint page, int row, int column, const char* s) {
    uint16* buffer = get_video_page_ptr(page);
    uint16* start = buffer + row * PAGE_WIDTH + column;
    int length = str_len(s);
    for (uint16* p = start; p < start + length; p++) {
        *p = *s++ | (WHITE_ON_BLACK << 8);
    } 
}
