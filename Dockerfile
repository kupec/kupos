FROM ubuntu:21.10

RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -yq \
    build-essential \
    dosfstools \
    ht \
    vim \
    gdb \
    parted \
    less

WORKDIR /kupos
CMD "make"
