FROM ubuntu:20.04

RUN apt update && apt install -y build-essential

WORKDIR /kupos
CMD "make"
