BOOT_SEG = 0x7c0
RELOC_SEG = 0x7e0
STACK_SEG = 0x800

PART_TABLE = 0x1be
PART_ENTRY_STATUS = 0
PART_ENTRY_OFFSET_LBA = 8
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
	push %es
	pop %ds

	call init_io

	/* find bootable partition */
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
	movw PART_ENTRY_OFFSET_LBA(%si), %bx
	movw $BOOT_SEG, %ax
	movw %ax, %es
	call read_sector

boot_sector_loaded:
	movw %bx, %ax

	pop %es
	pop %di
	pop %dx
	jmpl $BOOT_SEG,$0

no_boot_partition:
	movw $sNoBootPartition, %si
	call print_line
	jmp .
	
.include "boot.inc"

sNoBootPartition: .ascii "No boot partition found\r\n"
