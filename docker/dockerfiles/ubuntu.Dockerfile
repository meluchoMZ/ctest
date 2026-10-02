FROM ubuntu:stonking

USER root

RUN apt update && apt install -y gcc make libc6-dev
WORKDIR /home/ubuntu/ctest

USER 1000
