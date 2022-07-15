FROM ubuntu:21.10

RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -yq \
    build-essential \
    gcc-multilib \
    clang \
    dosfstools \
    ht \
    vim \
    gdb \
    parted \
    curl \
    less

WORKDIR /kupos
CMD "make"
