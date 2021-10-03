BOOT_SEG = 0x7c0
RELOC_SEG = 0x7e0
STACK_SEG = 0x800

PART_TABLE = 0x1be
PART_ENTRY_STATUS = 0
PART_ENTRY_HEAD = 1
PART_ENTRY_SECTOR = 2
PART_ENTRY_CYL = 3
PART_ENTRY_SIZE = 16
MAX_PART_COUNT = 4

SECTOR_SIZE = 512

.text
.code16

start:
	movw $STACK_SEG, %ax
	movw %ax, %ss
	movw $SECTOR_SIZE, %sp
	push %dx
	push %di
	push %es

	movw $BOOT_SEG, %ax
	movw %ax, %ds
	movw $RELOC_SEG, %ax
	movw %ax, %es
	xorw %si, %si
	xorw %di, %di
	movw $(SECTOR_SIZE/2), %cx
	cld
	rep movsw

	jmp $RELOC_SEG,$relocated

relocated:
	/* find bootable partition */
	push %es
	pop %ds
	movw $PART_TABLE, %si
	movw $MAX_PART_COUNT, %cx

next_partition:
	movb PART_ENTRY_STATUS(%si), %al
	andb $0x80, %al
	jnz load_boot_sector
	addw $PART_ENTRY_SIZE, %si
	decw %cx
	jz no_boot_partition
	jmp next_partition

load_boot_sector:
	movb PART_ENTRY_HEAD(%si), %dh
	movb PART_ENTRY_SECTOR(%si), %cl
	movb PART_ENTRY_CYL(%si), %ch
	xorw %bx, %bx
	movw $BOOT_SEG, %ax
	movw %ax, %es
	movw $0x0201, %ax
	int $0x13

	pop %es
	pop %di
	pop %di
	jmpl $BOOT_SEG,$0

no_boot_partition:
	movw $sNoBootPartition, %si
	call print_line
	jmp .
	
.include "boot.inc"

sNoBootPartition: .ascii "No boot partition found\r\n"
