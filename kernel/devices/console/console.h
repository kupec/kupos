#pragma once

#include "../../types/types.h"
#include "../../fd/interface.h"

void console_init();
FileDescriptor console_create_fd();
void console_print_line(char* s);
void console_print_line_f(const char* fmt, ...);
