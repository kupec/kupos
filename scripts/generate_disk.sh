#!/bin/bash
set -ex

OPT_FILE="$1"
OUTPUT="$2"

source "$OPT_FILE"
dd if=/dev/zero of="$OUTPUT" bs=1M count="$DISK_SIZE_IN_MB"
mkfs.fat "$OUTPUT"
