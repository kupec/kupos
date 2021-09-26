FROM ubuntu:20.04

RUN apt update && apt install -y build-essential dosfstools

WORKDIR /kupos
CMD "make"
