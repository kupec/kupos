#!/bin/bash
set -ex

OPT_FILE="$1"
OUTPUT="$2"

source "$OPT_FILE"
dd if=/dev/zero of="$OUTPUT" bs=1M count="$DISK_SIZE_IN_MB"
parted -s "$OUTPUT" -- \
    mklabel msdos \
    mkpart primary fat16 0 -1s \
    set 1 boot on
mkfs.fat --offset 1 -F 16 "$OUTPUT"

# TODO: delete it
# add garbage files
mkdir -p build/temp
mount -o offset=512 "$OUTPUT" build/temp
cp files/* build/temp
cp kernel/* build/temp
umount build/temp

