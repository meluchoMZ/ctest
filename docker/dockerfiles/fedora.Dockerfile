FROM fedora:latest

USER root

RUN dnf install -y gcc make git
WORKDIR /home/fedora/ctest

USER 1000
