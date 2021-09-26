.include "boot/code_opts.inc"

.text
.code16
. = BOOT_CODE_START

/* setup segment registers and stack */
boot:
	jmpl $0x07c0,$right_seg
right_seg:
	movw %cs, %ax
	movw %ax, %ds
	movw %ax, %es
	movw %ax, %ss
	movw $0x200, %sp

	movw $0x1300, %ax
	movw $0x0005, %bx
	movw $temp_sz, %cx
	xorw %dx, %dx
	movw $temp, %bp
	int $0x10

	cli
	hlt

temp: .ascii "hello world!"
temp_sz = . - temp

/* load main kernel to memory */

/* TODO */
