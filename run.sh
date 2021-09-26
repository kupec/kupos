#!/bin/sh
set -e

docker build -t kupos .
docker run -v "$PWD:/kupos" kupos make
qemu-system-i386 build/boot.img
