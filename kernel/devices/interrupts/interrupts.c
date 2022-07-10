#include "interrupts.h"
#include "../../types/types.h"
#include "../../asm/int.h"
#include "../../asm/io.h"
#include "../../int.h"

#define IDT_ENTRY_PRESENT 0x8000

#define PIC1_COMMAND 0x20
#define PIC1_DATA (PIC1_COMMAND + 1)
#define PIC1_START 0x20
#define PIC2_COMMAND 0xa0
#define PIC2_DATA (PIC2_COMMAND + 1)
#define PIC2_START 0x28

typedef struct {
    uint16 lo_offset;
    uint16 seg_selector;
    uint16 flags;
    uint16 hi_offset;
} IdtEntry;

struct IdtSpec {
    uint16 _;
    uint16 size;
    void* table;
};

IdtEntry idt[256];

void pic_remap(uint offset1, uint offset2) {
    outb(PIC1_COMMAND, 0x11);
    outb(PIC1_DATA, offset1);
    outb(PIC1_DATA, 4);
    outb(PIC1_DATA, 1);

    outb(PIC2_COMMAND, 0x11);
    outb(PIC2_DATA, offset2);
    outb(PIC2_DATA, 2);
    outb(PIC2_DATA, 1);

    inb(PIC1_DATA);
}

void interrupts_init() {
    struct IdtSpec idt_spec = {
        .size = 256 * sizeof(IdtEntry) - 1,
        .table = &idt,
    };
    __asm__ volatile (
        "lidt %0\n\t"
        :: "m" (idt_spec.size)
    );

    interrupts_set_vector(0, int_nop, INT_TRAP);
    interrupts_set_vector(1, int_nop, INT_TRAP);
    interrupts_set_vector(2, int_nop, INT_INT);
    interrupts_set_vector(3, int_nop, INT_TRAP);
    interrupts_set_vector(4, int_nop, INT_TRAP);
    interrupts_set_vector(5, int_nop, INT_TRAP);
    interrupts_set_vector(6, int_nop, INT_TRAP);
    interrupts_set_vector(7, int_nop, INT_TRAP);
    interrupts_set_vector(8, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(9, int_nop, INT_TRAP);
    interrupts_set_vector(10, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(11, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(12, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(13, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(14, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(15, int_nop, INT_TRAP);
    interrupts_set_vector(16, int_nop, INT_TRAP);
    interrupts_set_vector(17, int_with_code_nop, INT_TRAP);
    interrupts_set_vector(18, int_nop, INT_TRAP);
    interrupts_set_vector(19, int_nop, INT_TRAP);
    interrupts_set_vector(20, int_nop, INT_TRAP);
    interrupts_set_vector(21, int_with_code_nop, INT_TRAP);

    pic_remap(PIC1_START, PIC2_START);

    for (uint i = 0; i < 8; i++) {
        interrupts_set_vector(PIC1_START + i, irq_master_finish, INT_INT);
        interrupts_set_vector(PIC2_START + i, irq_slave_finish, INT_INT);
    }

    interrupts_set_vector(PIC1_START, irq_0, INT_INT);

    sti();
}

void interrupts_set_vector(uint vector, IntHandler handler, uint type) {
    cli();
    idt[vector].hi_offset = (uint32)handler >> 16;
    idt[vector].flags = IDT_ENTRY_PRESENT | (type << 8);
    idt[vector].seg_selector = 0x8;
    idt[vector].lo_offset = (uint32)handler & 0xFFFF;
    sti();
}

uint timer_ticks;

void irq_0_handler() {
    if (timer_ticks > 0) {
        timer_ticks--;
    }
}
