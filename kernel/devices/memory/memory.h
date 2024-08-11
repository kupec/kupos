#pragma once

#include "../../types/types.h"

#define MEM_PAGE_SIZE 0x1000

extern uint32 ram_size;

void memory_init();
int memory_alloc_pages(int count, void **out);
int memory_dealloc_pages(void* ptr, int count);
