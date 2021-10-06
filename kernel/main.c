#include "console.h"

void kernel_main() {
    console_print_line("Protected mode initialized");

    for (;;);
}
