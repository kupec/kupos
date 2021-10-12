#include "console.h"
#include "memory/memory.h"

void kernel_main() {
    console_print_line("Protected mode initialized");

    memory_init();
    console_print_line("Memory paging on");

    console_print_line("Trying to allocate memory...");
    void *ptr;
    if (memory_alloc_pages(1, &ptr) < 0) {
        console_print_line("Cannot allocate");
    }
    else {
        console_print_line("Allocated");
        console_print_line("Trying to access page");
        for (int i = 0; i < 1024; i++) {
            *(char*)ptr = i;
        }
        console_print_line("Good");
    }

    for (;;);
}
