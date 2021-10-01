/*
	print line:
	ds:si - string
 */
print_line:
	pushw %ax
	pushw %bx
1:
	lodsb
	test %al, %al
	jz 1f
	movb $0x0e, %ah
	movb $0x03, %bl
	int $0x10
	jmp 1b
1:
	popw %bx
	popw %ax
	ret

