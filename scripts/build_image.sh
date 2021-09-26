#!/bin/bash
set -ex

DISK_FILE="$1"
BOOT_SECTOR="$2"
OPT_FILE="$3"
OUTPUT="$4"

source "$OPT_FILE"
SKIP_BYTES=$(($BOOT_CODE_START))

BOOT_CODE_SIZE="$(du -b "$BOOT_SECTOR" | cut -f1)"
if [[ $BOOT_CODE_SIZE -gt 510 ]]; then
    echo "The boot code size should be less than 510 bytes" >&2
    exit 1;
fi;

cp "$DISK_FILE" "$OUTPUT.bak"
dd bs=1 if="$BOOT_SECTOR" skip="$SKIP_BYTES" of="$OUTPUT.bak" seek="$SKIP_BYTES" conv=notrunc
mv "$OUTPUT.bak" "$OUTPUT"
