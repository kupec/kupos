#include "devices/console/console.h"
#include "devices/memory/memory.h"
#include "devices/interrupts/interrupts.h"
#include "devices/timer/timer.h"
#include "devices/pci/pci.h"
#include "devices/disk/ata.h"
#include "std/number.h"
#include "std/string.h"
#include "std/format.h"
#include "debug/dump.h"
#include "asm/int.h"

void kernel_main() {
    console_init();
    FileDescriptor stdout = console_create_fd();

    fd_format(stdout, "Protected mode initialized");

    memory_init();
    fd_format(stdout, "Memory paging on");
    fd_format(stdout, "Memory size = %d MB", ram_size / 1024 / 1024);

    interrupts_init();
    fd_format(stdout, "Interrupts on");

    fd_format(stdout, "Trying to allocate memory...");
    volatile uint32 *ptr;
    if (memory_alloc_pages(1, (void*)&ptr) < 0) {
        fd_format(stdout, "Cannot allocate");
    }
    else {
        fd_format(stdout, "Allocated at 0x%x", ptr);
        fd_format(stdout, "Trying to access page");

        int mem_ok = 1;
        for (int i = 0; i < 1024; i++) {
            ptr[i] = i;
            if (ptr[i] != i) {
                mem_ok = 0;
                break;
            }
        }

        fd_format(stdout, mem_ok ? "Good" : "Bad");
    }

    fd_format(stdout, "Enumerating PCI devices...");
    PciSearch pci_search;
    pci_search_init(&pci_search);
    PciDevice pci_device;
    while (pci_search_next(&pci_search, &pci_device)) {
        if (pci_search.func == 0) {
            fd_format(stdout, "- %d.%d VEN=%x DEV=%x CLASS=%x(%s) SUBCLS=%x HDR=%x", pci_search.bus, pci_search.device, pci_device.vendor_id, pci_device.device_id, pci_device.class_code, PCI_CLASSES[pci_device.class_code], pci_device.subclass, pci_device.header_type);
        } else {
            fd_format(stdout, "  - FUNC=%x DEV=%x CLASS=%x(%s) SUBCLS=%x", pci_search.func, pci_device.device_id, pci_device.class_code, PCI_CLASSES[pci_device.class_code], pci_device.subclass);
        }
    }
    fd_format(stdout, "Done");

    uint16 ata_info[256];
    ata_init(ATA_DRIVE_MASTER, ata_info);
    fd_format(stdout, "Found ATA/IDE drive");
    int max_sectors = *(uint32*)(ata_info+60);
    fd_format(stdout, "- sectors=%d volume=%d MB", ata_info[60], ata_info[61]);
    char s[80];
    ata_convert_ascii(ata_info+10, 20, s);
    fd_format(stdout, "- serial number = %s", s);
    ata_convert_ascii(ata_info+23, 8, s);
    fd_format(stdout, "- firmware revision = %s", s);
    ata_convert_ascii(ata_info+27, 40, s);
    fd_format(stdout, "- model number = %s", s);
    print_hex(stdout, "ATA identify", ata_info, 0x40);

    char* sector_buf;
    memory_alloc_pages(1, (void*)&sector_buf);
    ata_read_sectors(ATA_DRIVE_MASTER, 0, 1, sector_buf);
    print_hex(stdout, "Boot sector", sector_buf, 0x20);
    memory_dealloc_pages(sector_buf, 1);

    for (;;) {
        hlt();
    }
}
