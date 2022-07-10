int cli_count = 1;

void cli() {
    __asm__ volatile ("cli");
    cli_count++;
}

void sti() {
    cli_count--;
    if (cli_count == 0) {
        __asm__ volatile ("sti");
    }
}

void hlt() {
    __asm__ volatile ("hlt");
}
