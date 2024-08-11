.text
.code32

kernel_entry:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movl $KERNEL_START, %esp

	jmp kernel_main
