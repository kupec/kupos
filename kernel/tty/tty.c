#include "tty.h"
#include "../std/math.h"
#include "../std/heap.h"
#include "../std/string.h"
#include "../devices/console/video.h"
#include "../devices/console/console.h"
#include "../devices/console/cursor.h"

void create_new_line(Tty *tty) {
    tty->lines[tty->line_count++] = "";
}

void append_current_line(Tty *tty, const char* text, int length) {
    char* prefix = tty->lines[tty->line_count-1];
    int prefix_length = str_len(prefix);

    int new_line_length = prefix_length + length;
    char* new_line = malloc(new_line_length + 1);
    tty->lines[tty->line_count-1] = new_line;

    for (int i = 0; i < prefix_length; i++) {
        new_line[i] = prefix[i];
    }
    free(prefix);

    for (int i = 0; i < length; i++) {
        new_line[prefix_length + i] = text[i];
    }
    new_line[new_line_length] = 0;
}

void write_to_buffer(Tty *tty, char *buffer, uint32 count) {
    char *line_start = buffer;
    char *end_buffer = buffer + count;
    for (char *p = buffer; p < end_buffer; p++) {
        if (*p == '\n') {
            append_current_line(tty, line_start, p - line_start);
            line_start = p + 1;
            create_new_line(tty);
            continue;
        }
    }
    if (end_buffer > line_start) {
        append_current_line(tty, line_start, end_buffer - line_start);
    }
}

void flush_buffer_to_console(Tty *tty) {
    uint current_page = video_get_page();
    uint next_page = 1 - current_page;

    console_clear_screen(next_page);
    int first_line_index = max(0, tty->line_count - tty->height + tty->scroll);
    int cursor_row = 0;
    int cursor_col = 0;
    for (int i = first_line_index; i < tty->line_count; i++) {
        int row = i - first_line_index;
        console_draw_text(next_page, row, 0, tty->lines[i]);
        cursor_row = row;
        cursor_col = str_len(tty->lines[i]);
    }

    video_set_page(next_page);
    console_cursor_set(cursor_row, cursor_col);
}

int tty_fd_write(void *data) {
    struct ServiceWriteData* write_data = (struct ServiceWriteData*)data;
    Tty *tty = write_data->state;

    write_to_buffer(tty, write_data->buffer, write_data->count);
    flush_buffer_to_console(tty);

    return E_OK;
}

Operation fd_operations[] = {
    tty_fd_write
};

void tty_init(Tty *tty) {
    FileDescriptor fd = {tty, fd_operations, LEN(fd_operations)};
    tty->fd = fd;
    tty->width = 80;
    tty->height = 25;

    tty->max_line_count = 100;
    tty->lines = malloc(tty->max_line_count * sizeof(char*));
    tty->line_count = 0;
    tty->scroll = 0;

    create_new_line(tty);
}
