#include "memory.h"
#include "../types/types.h"
#include "../asm/int.h"

extern uint32 MEMORY_FREE_REAL_MEM_STACK;
extern uint32 MEMORY_PAGE_STRUCTURES;

#define PAGE_STRUCTURES_ADDR (&MEMORY_PAGE_STRUCTURES)
#define ENTRY_COUNT 1024
#define PAGE_SIZE 0x1000
#define FREE_REAL_MEM_STACK_BUFFER (&MEMORY_FREE_REAL_MEM_STACK)
#define FREE_REAL_MEM_STACK_CAPACITY (ENTRY_COUNT * ENTRY_COUNT)

typedef struct {
    uint32 *buffer;
    uint32 top;
    uint32 capacity;
} Stack;

Stack free_real_mem_stack = {
    .buffer = FREE_REAL_MEM_STACK_BUFFER,
    .top = 0,
    .capacity = FREE_REAL_MEM_STACK_CAPACITY
};

int stack_push(Stack *stack, uint32 value) {
    if (stack->top == stack->capacity) {
        return -1;
    }

    stack->buffer[stack->top++] = value;
    return 0;
}

int stack_pop(Stack *stack, uint32 *out) {
    if (stack->top == 0) {
        return -1;
    }

    *out = stack->buffer[--stack->top];
    return 0;
}

uint32 stack_size(Stack *stack) {
    return stack->top;
}

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

uint32 *get_page_table_entry_ptr(void* virtual_ptr) {
    cli();

    uint32 virtual_addr = (uint32)virtual_ptr;
    if (virtual_addr & 0xFFF) {
        sti();
        return 0;
    }

    uint32 page_dir_index = virtual_addr >> 22;
    uint32 page_table_index = (virtual_addr >> 12) & 0x3FF;

    uint32 *page_dir = get_page_dir_ptr();
    uint32 *page_table = (uint32*)(page_dir[page_dir_index] & 0xFFFFF000);

    uint32 *result = &page_table[page_table_index];

    sti();
    return result;
}

int memory_map_page(void* virtual_ptr, void* real_ptr) {
    cli();

    uint32 virtual_addr = (uint32)virtual_ptr;
    uint32 real_addr = (uint32)real_ptr;
    if ((virtual_addr & 0xFFF) | (real_addr & 0xFFF)) {
        sti();
        return -1;
    }

    uint32 *page_entry_ptr = get_page_table_entry_ptr(virtual_ptr);
    *page_entry_ptr = real_addr + 0x003;

    sti();
    return 0;
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
    for (void *ptr = 0; ptr < ptr_after_page_tables; ptr += PAGE_SIZE) {
        memory_map_page(ptr, ptr);
    }

    enable_paging(page_dir);

    for (uint32 i = 0xfffff000; i >= (uint32)ptr_after_page_tables; i -= PAGE_SIZE) {
        stack_push(&free_real_mem_stack, i);
    }
}

int memory_unmap_page(void* ptr) {
    cli();

    uint32 *page_entry_ptr = get_page_table_entry_ptr(ptr);
    if (!page_entry_ptr) {
        sti();
        return -1;
    }

    *page_entry_ptr = 0;

    sti();
    return 0;
}

int find_free_pages(int count, void **out) {
    int found_page_count = 0;
    for (uint32 i = 0; i < ENTRY_COUNT; i++) {
        for (uint32 j = 0; j < ENTRY_COUNT; j++) {
            void *ptr = (void*)((i << 22) + (j << 12));
            uint32 *page_entry_ptr = get_page_table_entry_ptr(ptr);
            if ((*page_entry_ptr & 1) == 1) {
                found_page_count = 0;
            }
            else {
                if (found_page_count == 0) {
                    *out = ptr;
                }
                if (++found_page_count >= count) {
                    return 0;
                }
            }
        }
    }

    return -1;
}

int memory_alloc_pages(int count, void **out) {
    if (stack_size(&free_real_mem_stack) < count) {
        return -1;
    }

    if (find_free_pages(count, out) == -1) {
        return -1;
    }

    void *virtual_ptr = *out;
    for (int i = 0; i < count; i++) {
        uint32 real_addr;
        stack_pop(&free_real_mem_stack, &real_addr);

        memory_map_page(virtual_ptr + i * PAGE_SIZE, (void*)real_addr);
    }

    return 0;
}

int memory_dealloc_pages(void* ptr, int count) {
    for (int i = 0; i < count; i++) {
        uint32 *page_entry_ptr = get_page_table_entry_ptr(ptr);
        uint32 real_ptr = *page_entry_ptr & 0xFFFFF000;
        stack_push(&free_real_mem_stack, real_ptr);

        memory_unmap_page(ptr);
        ptr += PAGE_SIZE;
    }

    return 0;
}
