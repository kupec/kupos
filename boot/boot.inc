IO_TRY_COUNT = 3

/*
    print line:
    ds:si - string
 */
print_line:
    pushaw
1:
    lodsb
    test %al, %al
    jz 1f
    movb $0x0e, %ah
    movb $0x03, %bl
    int $0x10
    jmp 1b
1:
    popaw
    ret

/*
    init_io
    reset io and prepare for read/write (calculate disk geometry):
    dl - drive
 */
init_io:
    pushaw

    movb %dl, drive
    movb IO_TRY_COUNT, %bl
1:
    xorw %ax, %ax
    int $0x13
    movb $8, %ah
    movb drive, %dl
    int $0x13
    jnc 1f

    /* io error, trying again */
    decb %bl
    jnz 1b
2:
    /* io fatal error */
    movw $sIoError, %si
    call print_line
    jmp .

1:
    incb %dh
    movb %dh, head_per_track
    andb $0x3f, %cl
    movb %cl, sector_per_head

    popaw
    ret

/*
    read sector:
    bx - LBA (starting with 0)
    es - the sector will be read to es:0
 */
read_sector:
    pushaw

    /* LBA -> CHS - S part */
    xorw %dx, %dx
    movw %bx, %ax
    divw sector_per_head
    movw %dx, %cx
    incw %cx

    /* LBA -> CHS - H part */
    xorw %dx, %dx
    divw head_per_track
    movb %dl, %dh
    movb drive, %dl

    /* LBA -> CHS - C part */
    movb %al, %ch
    shlb $6, %ah
    orb %ah, %cl

    movw $0x0201, %ax
    xorw %bx, %bx
    int $0x13

    popaw
    ret

drive: .byte 0
sector_per_head: .word 0
head_per_track: .word 0
sIoError: .asciz "IO Error\r\n"
