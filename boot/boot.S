.include "opts.inc"
.include "../kernel/kernel.inc"

BPB_BYTES_PER_SECTOR = 0xb
BPB_SECTOR_PER_CLUSTER = 0xd
BPB_RESERVED_SECTORS = 0xe
BPB_FAT_COUNT = 0x10
BPB_DIR_ENTRY_COUNT = 0x11
BPB_FAT_SECTORS = 0x16

STACK_SEG = 0x7c0
SECTOR_SEG = 0x7e0
MAIN_SEG = 0x800

SECTOR_SIZE = 512
SECTOR_SIZE_SEG_INC = SECTOR_SIZE / 16
DIR_ENTRY_SIZE = 32
DIR_ENTRY_SIZE_LOG = 5
DIR_ENTRY_PER_SECTOR = SECTOR_SIZE / DIR_ENTRY_SIZE
DIR_ENTRY_FIRST_CLUSTER = 0x1a
DIR_ENTRY_FILENAME_LEN = 11

FAT_ENTRY_SIZE = 2
FAT_ENTRY_PER_SECTOR = SECTOR_SIZE / FAT_ENTRY_SIZE
FAT_EOF = 0xfff8

KERNEL_START_SEG = KERNEL_START / 0x10

.text
.code16
. = BOOT_CODE_START

boot:
	/* use initial boot sector as stack, copy code to next 0x200 bytes */
	movw $STACK_SEG, %bx
	movw %bx, %ds
	movw %bx, %ss
	movw $SECTOR_SIZE, %sp

	movw %ax, partition_first_sector

	movw $MAIN_SEG, %ax
	movw %ax, %es
	xorw %si, %si
	xorw %di, %di
	movw $(SECTOR_SIZE/2), %cx
	cld
	rep movsw

	jmpl $MAIN_SEG,$boot2
boot2:
	/* setup segment registers and stack */
	movw %ax, %ds
	movw %ax, %es

	/* find kernel file */
	movw $sSearchingKernel, %si
	call print_line

	call init_io

	movw $SECTOR_SEG, %ax
	movw %ax, %es

	movw BPB_FAT_SECTORS, %ax
	movzbw BPB_FAT_COUNT, %dx
	mulw %dx
	addw BPB_RESERVED_SECTORS, %ax
	addw partition_first_sector, %ax
	movw %ax, root_dir_sector_offset
	movw %ax, %bx
	movw BPB_DIR_ENTRY_COUNT, %bp

root_dir_sector:
	call read_sector
	xorw %di, %di
1:
	pushw %di
	movw $kernel_name, %si
	movw $DIR_ENTRY_FILENAME_LEN, %cx
	rep cmpsb
	popw %di
	jz kernel_found
	decw %bp
	jz kernel_not_found
	addw $DIR_ENTRY_SIZE, %di
	cmp $SECTOR_SIZE, %di
	jl 1b

	incw %bx
	jmp root_dir_sector

kernel_not_found:
	movw $sCannotFoundKernel, %si
	call print_line
	jmp .

kernel_found:
	/* loading kernel, es:di -> kernel dir entry */
	movw $sLoadingKernel, %si
	call print_line

	movw BPB_DIR_ENTRY_COUNT, %ax
	shlw $DIR_ENTRY_SIZE_LOG, %ax
	addw $(DIR_ENTRY_SIZE - 1), %ax
	divw BPB_BYTES_PER_SECTOR
	addw root_dir_sector_offset, %ax
	movw %ax, %bp

	movw %es:DIR_ENTRY_FIRST_CLUSTER(%di), %ax

next_cluster:
	/* save cluster index */
	push %ax

	subw $2, %ax
	movzbw BPB_SECTOR_PER_CLUSTER, %dx
	mulw %dx

	movw %ax, %bx
	addw %bp, %bx
	movw kernel_cluster_seg, %ax
	movw %ax, %es
	movzbw BPB_SECTOR_PER_CLUSTER, %cx
read_cluster:
	call read_sector
	incw %bx
	movw %es, %ax
	addw $SECTOR_SIZE_SEG_INC, %ax
	movw %ax, %es
	loop read_cluster

	/* restore cluster index */
	popw %ax
	xorw %dx, %dx
	movw $FAT_ENTRY_PER_SECTOR, %cx
	divw %cx
	movw %dx, %di

	movw BPB_RESERVED_SECTORS, %bx
	addw partition_first_sector, %bx
	addw %ax, %bx
	movw $SECTOR_SEG, %ax
	movw %ax, %es
	call read_sector

	shlw $1, %di
	movw %es:(%di), %ax
	cmpw $FAT_EOF, %ax
	jae run_kernel
	movw %ax, %bx
	movw $SECTOR_SIZE_SEG_INC, %ax
	mulb BPB_SECTOR_PER_CLUSTER
	addw %ax, kernel_cluster_seg
	movw %bx, %ax
	jmp next_cluster

run_kernel:
	jmpl $KERNEL_START_SEG,$0

.include "boot.inc"

sSearchingKernel: .asciz "Find kernel\r\n"
sCannotFoundKernel: .asciz "Not found kernel.bin\r\n"
sLoadingKernel: .asciz "Load kernel\r\n"

partition_first_sector: .word 0
kernel_name: .ascii "KERNEL  BIN"
root_dir_sector_offset: .word 0
kernel_cluster_seg: .word KERNEL_START_SEG
