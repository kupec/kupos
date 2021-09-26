#!/bin/sh
set -e

docker build -t kupos .
docker run -it -v "$PWD:/kupos" kupos bash
