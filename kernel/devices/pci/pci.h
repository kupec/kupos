#pragma once

#include "../../types/types.h"
#include <stdbool.h>

typedef struct {
    uint16 vendor_id;
    uint16 device_id;
    uint16 command;
    uint16 status;
    uint8 revision_id;
    uint8 prog_if;
    uint8 subclass;
    uint8 class_code;
    uint8 cache_line_size;
    uint8 latency_timer;
    uint8 header_type;
    uint8 bist;
} PciDevice;

typedef struct {
    int bus;
    int device;
    int func;
    bool multi_func;
} PciSearch;

extern char* PCI_CLASSES[];

void pci_search_init(PciSearch *search);
bool pci_search_next(PciSearch *search, PciDevice* out);
bool pci_check_device(uint8 bus, uint8 device, uint8 func, PciDevice* out);
uint32 pci_config_read_dword(uint8 bus, uint8 device, uint8 func, uint8 offset);
uint16 pci_config_read_word(uint8 bus, uint8 device, uint8 func, uint8 offset);
