#!/bin/bash
set -ex

OPT_FILE="$1"
OUTPUT="$2"

source "$OPT_FILE"
dd if=/dev/zero of="$OUTPUT" bs=1M count="$DISK_SIZE_IN_MB"
parted -s "$OUTPUT" -- \
    mklabel msdos \
    mkpart primary fat16 0 $(($BOOT_SECTOR_LBA - 1))s \
    mkpart primary fat16 ${BOOT_SECTOR_LBA}s -1s \
    set 2 boot on
mkfs.fat --offset $BOOT_SECTOR_LBA -F 16 "$OUTPUT"

# TODO: move it to another script
mkdir -p build/temp
mount -o offset=$(($BOOT_SECTOR_LBA * 512)) "$OUTPUT" build/temp
cp kernel/kernel.bin build/temp
umount build/temp

