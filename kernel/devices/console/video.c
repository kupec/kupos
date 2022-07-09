#include "video.h"
#include "../../types/types.h"
#include "../../asm/io.h"

uint current_page = 0;

uint video_get_page() {
    return current_page;
}

void video_select_page(uint page) {
    uint16 offset = page * PAGE_SIZE;

    outb(CRT_REG_PORT, CRT_MEM_START_REG_HI);
    outb(CRT_VAL_PORT, offset >> 8);
    outb(CRT_REG_PORT, CRT_MEM_START_REG_LO);
    outb(CRT_VAL_PORT, offset);
}

void video_set_page(uint page) {
    if (current_page == page) {
        return;
    }

    video_select_page(page);
    current_page = page;
}
