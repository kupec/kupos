#!/bin/bash
set -e

objdump -D -b binary -mi386 -Maddr16 -Mdata16 "$1"
