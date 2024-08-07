#pragma once

#include "../../types/types.h"
#include <stdbool.h>

void ata_init(void *buffer);
void ata_convert_ascii(const void* input, int len, char* output);
