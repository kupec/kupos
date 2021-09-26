#!/bin/bash
set -e

source docker.inc

docker build -t kupos .
docker run $DOCKER_FLAGS -it kupos bash
