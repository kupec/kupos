#!/bin/sh
set -e

docker build -t kupos .
docker run --rm -it -v "$PWD:/kupos" kupos bash
