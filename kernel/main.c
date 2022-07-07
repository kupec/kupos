#include "console.h"
#include "memory/memory.h"
#include "string/number.h"
#include "string/string.h"

void kernel_main() {
    console_print_line("Protected mode initialized");

    memory_init();
    console_print_line("Memory paging on");

    console_print_line_f("Memory size = %d MB", ram_size / 1024 / 1024);

    console_print_line("Trying to allocate memory...");
    uint32 *ptr;
    if (memory_alloc_pages(1, (void*)&ptr) < 0) {
        console_print_line("Cannot allocate");
    }
    else {
        console_print_line_f("Allocated at 0x%x", ptr);
        console_print_line("Trying to access page");

        int mem_ok = 1;
        for (int i = 0; i < 1024; i++) {
            ptr[i] = i;
            if (ptr[i] != i) {
                mem_ok = 0;
                break;
            }
        }

        console_print_line(mem_ok ? "Good" : "Bad");
    }

    for (;;);
}
