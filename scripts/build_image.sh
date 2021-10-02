#!/bin/bash
set -ex

DISK_FILE="$1"
MBR_SECTOR="$2"
BOOT_SECTOR="$3"
OPT_FILE="$4"
OUTPUT="$5"

source "$OPT_FILE"
SKIP_BYTES=$(($BOOT_CODE_START))

MBR_CODE_SIZE="$(du -b "$MBR_SECTOR" | cut -f1)"
if [[ $MBR_CODE_SIZE -gt 446 ]]; then
    echo "The mbr code size should be less than 446 bytes" >&2
    exit 1;
fi;

BOOT_CODE_SIZE="$(du -b "$BOOT_SECTOR" | cut -f1)"
if [[ $BOOT_CODE_SIZE -gt 510 ]]; then
    echo "The boot code size should be less than 510 bytes" >&2
    exit 1;
fi;

cp "$DISK_FILE" "$OUTPUT.bak"
dd bs=1 if="$MBR_SECTOR" of="$OUTPUT.bak" conv=notrunc
let "SEEK = SKIP_BYTES + 512"
dd bs=1 if="$BOOT_SECTOR" skip="$SKIP_BYTES" of="$OUTPUT.bak" seek="$SEEK" conv=notrunc
mv "$OUTPUT.bak" "$OUTPUT"
