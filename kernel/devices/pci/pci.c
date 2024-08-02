#include "pci.h"
#include "../../asm/io.h"

#define CONFIG_ADDR_PORT 0xcf8
#define CONFIG_DATA_PORT 0xcfc

#define CONFIG_ADDR_ENABLE 0x80000000
#define CONFIG_HEADER_MULTI 0x80

char* PCI_CLASSES[] = {
    "Unclassified",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge",
    "Simple Communication Controller",
    "Base System Peripheral",
    "Input Device Controller",
    "Docking Station",
    "Processor",
    "Serial Bus Controller",
    "Wireless Controller",
    "Intelligent Controller",
    "Satellite Communication Controller",
    "Encryption Controller",
    "Signal Processing Controller",
    "Processing Accelerator",
    "Non-Essential Instrumentation",
};

uint32 pci_config_read_dword(uint8 bus, uint8 device, uint8 func, uint8 offset) {
    uint32 addr = CONFIG_ADDR_ENABLE | (bus << 16) | ((device & 0x1f) << 11) | ((func & 0x7) << 8) | (offset & 0xfc);
    outl(CONFIG_ADDR_PORT, addr);
    return inl(CONFIG_DATA_PORT);
}

uint16 pci_config_read_word(uint8 bus, uint8 device, uint8 func, uint8 offset) {
    uint32 dword = pci_config_read_dword(bus, device, func, offset & 0xfc);
    return ((offset & 0x3) == 0) ? dword : dword >> 16;
}

bool pci_check_device(uint8 bus, uint8 device, uint8 func, PciDevice* out) {
    uint32 data = pci_config_read_dword(bus, device, func, 0);
    if (((PciDevice*)(&data))->device_id == 0xffff) {
        return false;
    }

    uint32* ptr = (uint32*)out;
    ptr[0] = data;

    uint dword_count = sizeof(PciDevice) / sizeof(uint32);
    for (uint i = 1; i < dword_count; i++) {
        uint offset = i * sizeof(uint32);
        ptr[i] = pci_config_read_dword(bus, device, 0, offset);
    }

    return true;
}

void pci_search_init(PciSearch *search) {
    search->bus = -1;
    search->device = 0;
    search->func = 0;
    search->multi_func = false;
}

bool pci_search_next_increment(PciSearch* search) {
    if (search->bus == -1) {
        search->bus = 0;
        return true;
    }

    if (search->multi_func) {
        search->func = (search->func + 1) % 8;
        if (search->func != 0) {
            return true;
        }
    }

    search->device = (search->device + 1) % 32;
    if (search->device != 0) {
        return true;
    }

    search->bus = (search->bus + 1) % 256;
    return search->bus != 0;
}

bool pci_search_next(PciSearch* search, PciDevice* out) {
    while (true) {
        if (!pci_search_next_increment(search)) {
            return false;
        }

        if (pci_check_device(search->bus, search->device, search->func, out)) {
            break;
        }
    }

    search->multi_func = (out->header_type & CONFIG_HEADER_MULTI) != 0;
    return true;
}
