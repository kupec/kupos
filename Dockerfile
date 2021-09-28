FROM ubuntu:20.04

RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -yq build-essential dosfstools ht vim gdb

WORKDIR /kupos
CMD "make"
