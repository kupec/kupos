#pragma once

#include "../types/types.h"
#include <stdbool.h>

int heap_init();
void* malloc(int size);
void free(void* ptr);
