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

#define CMD_IDENTIFY 0xEC
#define CMD_READ_SECTORS 0x21

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

void ata_init(int drive, void *buffer) {
    wait_for_drive_ready();
    wait_for_not_busy();

    select_drive(drive);
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

void set_lba(int drive, uint32 lba) {
    outb(reg(DRIVE_HEAD_REG), (lba >> 24 & 0x0F | (0xE0 + (drive << 4))));
    outb(reg(CYLINDER_HI_REG), (lba >> 16) & 0xFF);
    outb(reg(CYLINDER_LO_REG), (lba >> 8) & 0xFF);
    outb(reg(SECTOR_NUMBER_REG), lba & 0xFF);
}

void ata_read_sectors(int drive, uint32 lba, uint8 sector_count, void* buf) {
    wait_for_not_busy();

    select_drive(drive);
    set_lba(drive, lba);
    outb(reg(SECTOR_COUNT_REG), sector_count);

    execute_command(CMD_READ_SECTORS);
    while (sector_count--) {
        read_block(buf);
        buf += SECTOR_SIZE;
    }
}
