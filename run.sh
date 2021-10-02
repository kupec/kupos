#!/bin/bash
set -e

source docker.inc

docker build -t kupos .
docker run $DOCKER_FLAGS kupos make
qemu-system-i386 -hda build/kupos.img
