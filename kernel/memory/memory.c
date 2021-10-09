#include "memory.h"
#include "../types/types.h"
#include "../asm/int.h"

extern uint32 KERNEL_START;
extern uint32 KERNEL_SIZE;

#define PAGE_STRUCTURES_ADDR ((uint32*)((void*)&KERNEL_START + (uint32)&KERNEL_SIZE))
#define ENTRY_COUNT 1024
#define PAGE_SIZE 0x1000

void enable_paging(void *page_dir) {
    __asm__ volatile (
        "movl %0, %%cr3\n\t"
        "movl %%cr0, %%eax\n\t"
        "btsl $31, %%eax\n\t"
        "movl %%eax, %%cr0\n\t"
        :: "r" (page_dir)
        : "eax", "cc"
    );
}

uint32 *get_page_dir_ptr() {
    return PAGE_STRUCTURES_ADDR;
}

void memory_cache_invalidate(void *page) {
    __asm__ volatile ("invlpg %0" :: "m" (page));
}

void memory_init() {
    uint32 *page_dir = get_page_dir_ptr();

    for (int i = 0; i < ENTRY_COUNT; i++) {
        uint32 *page_table = (void*)page_dir + (i + 1) * PAGE_SIZE;
        page_dir[i] =  (uint32)page_table + 0x003;

        for (int j = 0; j < ENTRY_COUNT; j++) {
            page_table[i] = 0;
        }
    }

    void *ptr_after_page_tables = page_dir + (1 + ENTRY_COUNT) * ENTRY_COUNT;
    for (void *ptr = &KERNEL_START; ptr < ptr_after_page_tables; ptr += PAGE_SIZE) {
        memory_map_page(ptr, ptr);
    }

    enable_paging(page_dir);
}

int memory_map_page(void* virtual_ptr, void* real_ptr) {
    cli();

    uint32 virtual_addr = (uint32)virtual_ptr;
    uint32 real_addr = (uint32)real_ptr;
    if ((virtual_addr & 0xFFF) | (real_addr & 0xFFF)) {
        sti();
        return -1;
    }

    uint32 page_dir_index = virtual_addr >> 22;
    uint32 page_table_index = (virtual_addr >> 12) & 0x3FF;

    uint32 *page_dir = get_page_dir_ptr();
    uint32 *page_table = (uint32*)(page_dir[page_dir_index] & 0xFFFFF000);

    page_table[page_table_index] = real_addr + 0x001;

    sti();
    return 0;
}
