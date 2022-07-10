.text

PIC1_COMMAND = 0x20
PIC1_DATA = PIC1_COMMAND + 1
PIC2_COMMAND = 0xa0
PIC2_DATA = PIC2_COMMAND + 1
PIC_EOI = 0x20

.global int_with_code_nop
int_with_code_nop:
	add $4, %esp
.global int_nop
int_nop:
	iret

.global irq_slave_finish
irq_slave_finish:
	movb $PIC_EOI, %al
	outb %al, $PIC2_COMMAND
.global irq_master_finish
irq_master_finish:
	movb $PIC_EOI, %al
	outb %al, $PIC1_COMMAND
	iret

.global irq_0
irq_0:
	pushl %eax
	pushl %ecx
	pushl %edx

	call irq_0_handler

	popl %edx
	popl %ecx
	popl %eax
	jmp irq_master_finish
