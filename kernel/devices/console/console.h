#pragma once

#include "../../types/types.h"

void console_init();
void console_clear_screen(uint page);
void console_draw_text(uint page, int row, int column, const char* s);
