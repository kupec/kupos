#pragma once

#include "../../types/types.h"

#define INT_INT 0x0e
#define INT_TRAP 0x0f

typedef void IntHandler();

void interrupts_init();
void interrupts_set_vector(uint vector, IntHandler handler, uint type);
