#include "console.h"
#include "memory/memory.h"

void kernel_main() {
    console_print_line("Protected mode initialized");

    memory_init();
    console_print_line("Memory paging on");

    for (;;);
}
