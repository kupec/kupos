#!/bin/sh
set -e

docker build -t kupos .
docker run --rm -v "$PWD:/kupos" kupos make
qemu-system-i386 build/kupos.img
