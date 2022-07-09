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

void console_print_line(char* s) {
    uint16 row, col;
    console_cursor_get(&row, &col);

    int scroll_line_count = max(0, row + 2 - PAGE_HEIGHT);
    int start_pos = scroll_line_count * PAGE_WIDTH;
    int end_pos = row * PAGE_WIDTH;
    row -= scroll_line_count;

    uint next_page = 1 - video_get_page();
    uint16* current_buffer = get_video_page_ptr(video_get_page());
    uint16* next_buffer = get_video_page_ptr(next_page);

    console_clear_screen(next_page);
    for (int i = start_pos; i < end_pos; i++) {
        next_buffer[i - start_pos] = current_buffer[i];
    } 

    int len = min(str_len(s), PAGE_WIDTH - col);
    int cursor = row * PAGE_WIDTH + col;

    for (int i = 0; i < len; i++) {
        ((char*)next_buffer)[(cursor + i) * 2] = s[i];
    }

    video_set_page(next_page);
    console_cursor_set(row + 1, 0);
}

void console_print_line_f(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char s[80];
    vformat(fmt, LEN(s), s, args);
    console_print_line(s);

    va_end(args);
}

int console_fd_write(void *data) {
    struct ServiceWriteData* writeData = (struct ServiceWriteData*)data;
    console_print_line(writeData->buffer);
    return E_OK;
}

Operation fd_operations[] = {
    console_fd_write
};

FileDescriptor console_fd = {0, fd_operations, LEN(fd_operations)};

FileDescriptor console_create_fd() {
    return console_fd;
}
