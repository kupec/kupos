.include "kernel.inc"

GDT_SIZE = 10

.text
.code16

	pushw %cs
	popw %ds
	movw $sGoToProtectedMode, %si
	call print_line

	/* prepare to protected mode */

	cli

	/* cli for non maskable interrupt */
	//???

	/* open a20 gate */
	//???

	lgdt pGDT
	movl %cr0, %eax
	orl $1, %eax
	movl %eax, %cr0
	jmpl $8,$KERNEL_ENTRYPOINT

.include "../boot/boot.inc"

sGoToProtectedMode: .asciz "Going to protected mode...\r\n"

.align 8
GDT:
	/* reserved */
	.quad 0
	/* code */
	.byte 0xff, 0xff, 0, 0
	.byte 0, 0x9e, 0xcf, 0
	/* data */
	.byte 0xff, 0xff, 0, 0
	.byte 0, 0x92, 0xcf, 0
.rept GDT_SIZE - 3
	.quad 0
.endr

pGDT:
	.2byte GDT_SIZE * 8 - 1
	.4byte (0x10000 + GDT)

.align 0x1000
