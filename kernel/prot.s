.text
.code32

kernel_entry:
	movw $0x10, %ax
	movw %ax, %ds
	movw %ax, %ss
	movw %ax, %es
	jmp kernel_main
