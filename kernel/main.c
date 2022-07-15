#include "devices/console/console.h"
#include "devices/memory/memory.h"
#include "devices/interrupts/interrupts.h"
#include "devices/timer/timer.h"
#include "std/number.h"
#include "std/string.h"
#include "std/format.h"
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

    int sleep_count = 10;
    fd_format(stdout, "Wait for %d seconds", sleep_count);
    for (int i = 0; i < sleep_count; i++) {
        sleep(1000);
        fd_format(stdout, "%d seconds elapsed", i+1);
    }

    for (;;) {
        hlt();
    }
}
