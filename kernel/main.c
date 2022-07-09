#include "devices/console/console.h"
#include "devices/memory/memory.h"
#include "std/number.h"
#include "std/string.h"
#include "std/format.h"

void kernel_main() {
    FileDescriptor stdout = console_create_fd();

    fd_format(stdout, "Protected mode initialized");

    memory_init();
    fd_format(stdout, "Memory paging on");

    fd_format(stdout, "Memory size = %d MB", ram_size / 1024 / 1024);

    fd_format(stdout, "Trying to allocate memory...");
    uint32 *ptr;
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

    for (;;);
}
