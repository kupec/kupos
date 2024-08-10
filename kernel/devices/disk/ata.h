#pragma once

#include "../../types/types.h"
#include <stdbool.h>

#define ATA_DRIVE_MASTER 0
#define ATA_DRIVE_SLAVE 1

void ata_init(int drive, void *buffer);
void ata_read_sectors(int drive, uint32 lba, uint8 sector_count, void* buf);
void ata_convert_ascii(const void* input, int len, char* output);
