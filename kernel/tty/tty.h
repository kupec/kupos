#pragma once

#include "../types/types.h"
#include "../fd/interface.h"

typedef struct {
    FileDescriptor fd;
    int width;
    int height;
    char** lines;
    int line_count;
    int max_line_count;
    int scroll;
} Tty;

void tty_init(Tty *tty);
