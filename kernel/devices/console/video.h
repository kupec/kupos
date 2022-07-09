#pragma once

#include "../../types/types.h"

#define PAGE_WIDTH 80
#define PAGE_HEIGHT 25
#define PAGE_SIZE (PAGE_WIDTH * PAGE_HEIGHT)
#define VIDEO_BUFFER (uint16 *)0xb8000

#define WHITE_ON_BLACK 0x07

#define CRT_REG_PORT 0x3d4
#define CRT_VAL_PORT 0x3d5
#define CRT_MEM_START_REG_HI 12
#define CRT_MEM_START_REG_LO 13
#define CRT_CURSOR_POS_REG_HI 14
#define CRT_CURSOR_POS_REG_LO 15

uint video_get_page();
void video_set_page(uint page);
