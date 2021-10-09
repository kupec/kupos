#!/bin/bash
set -e

source docker.inc

docker build -t kupos .
docker run $DOCKER_FLAGS kupos make
qemu-system-i386 -S -s -hda build/kupos.img &
QEMU_PID="$!"
docker run $DOCKER_FLAGS -it kupos gdb -x scripts/debug.gdb
kill "$QEMU_PID"
