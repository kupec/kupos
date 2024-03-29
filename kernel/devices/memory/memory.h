#pragma once

#include "../../types/types.h"

extern uint32 ram_size;

void memory_init();
int memory_alloc_pages(int count, void **out);
int memory_dealloc_pages(void* ptr, int count);
