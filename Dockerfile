FROM ubuntu:21.10

RUN apt update && DEBIAN_FRONTEND=noninteractive apt install -yq \
    build-essential \
    gcc-multilib \
    dosfstools \
    ht \
    vim \
    gdb \
    parted \
    curl \
    less
RUN curl --proto '=https' --tlsv1.2 -sSf \
       https://sh.rustup.rs > setup_rust.sh
RUN sh setup_rust.sh -y --default-host i686-unknown-linux-gnu --default-toolchain nightly

WORKDIR /kupos
CMD "make"
