#include "console.h"
#include "memory/memory.h"
#include "string/number.h"
#include "string/string.h"

void kernel_main() {
    console_print_line("Protected mode initialized");

    memory_init();
    console_print_line("Memory paging on");

    char ram_size_s[10];
    num_to_str(ram_size / 1024 / 1024, 10, ram_size_s);
    char buffer[80];
    str_cat("Memory size = ", ram_size_s, 80, buffer);
    str_cat(buffer, " MB", 80, buffer);
    console_print_line(buffer);

    console_print_line("Trying to allocate memory...");
    uint32 *ptr;
    if (memory_alloc_pages(1, (void*)&ptr) < 0) {
        console_print_line("Cannot allocate");
    }
    else {
        console_print_line("Allocated");
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
