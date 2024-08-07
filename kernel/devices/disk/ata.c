#include "ata.h"
#include "../../asm/io.h"

#define CMD_BLOCK_PORT_BASE 0x1f0

#define DATA_REG 0
#define ERROR_REG 1
#define FEATURE_REG 1
#define SECTOR_COUNT_REG 2
#define SECTOR_NUMBER_REG 3
#define CYLINDER_LO_REG 4
#define CYLINDER_HI_REG 5
#define DRIVE_HEAD_REG 6
#define STATUS_REG 7
#define COMMAND_REG 7

#define STATUS_BUSY 0x80
#define STATUS_REQUEST 0x08
#define STATUS_DRIVE_READY 0x40

#define DRIVE_SHIFT 4
#define DRIVE_MASK (1 << DRIVE_SHIFT)
#define DRIVE_MASTER 0
#define DRIVE_SLAVE 1

#define CMD_IDENTIFY 0xEC

#define SECTOR_SIZE 0x200

#define reg(A) (CMD_BLOCK_PORT_BASE + (A))

int read_status() {
    return inb(reg(STATUS_REG));
}

void wait_for_drive_ready() {
    while ((read_status() & STATUS_DRIVE_READY) == 0);
}

void wait_for_request() {
    while ((read_status() & STATUS_REQUEST) == 0);
}

void wait_for_not_busy() {
    while ((read_status() & STATUS_DRIVE_READY) != 0);
}

void select_drive(int drive) {
    char value = inb(reg(DRIVE_HEAD_REG));
    outb(reg(DRIVE_HEAD_REG), (value & ~DRIVE_MASK) | (drive << DRIVE_SHIFT));
}

void execute_command(int cmd) {
    wait_for_not_busy();
    outb(reg(COMMAND_REG), cmd);
}

void read_block(void *buffer) {
    wait_for_request();
    insw(reg(DATA_REG), buffer, SECTOR_SIZE/2);
}

void ata_init(void *buffer) {
    wait_for_drive_ready();

    select_drive(DRIVE_MASTER);
    execute_command(CMD_IDENTIFY);
    read_block(buffer);
}

void ata_convert_ascii(const void* input, int len, char* output) {
    while (len-=2) {
        uint8 lo = *(char*)input++;
        uint8 hi = *(char*)input++;
        *output++ = hi;
        *output++ = lo;
    }
    *output = 0;
}
