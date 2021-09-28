#!/bin/bash
set -ex

OPT_FILE="$1"
OUTPUT="$2"

source "$OPT_FILE"
dd if=/dev/zero of="$OUTPUT" bs=1M count="$DISK_SIZE_IN_MB"
mkfs.fat "$OUTPUT"

# TODO: delete it
# add garbage files
mkdir -p build/temp
mount "$OUTPUT" build/temp
cp files/* build/temp
cp kernel/* build/temp
umount build/temp

