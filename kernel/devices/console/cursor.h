#pragma once

#include "../../types/types.h"

void console_cursor_init();
void console_cursor_set(uint16 row, uint16 col);
void console_cursor_get(uint16 *row, uint16 *col);
