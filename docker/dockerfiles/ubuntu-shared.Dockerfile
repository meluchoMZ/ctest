FROM ctest-base:ubuntu

COPY . /tmp
WORKDIR /tmp

USER root
RUN LIB_DIR=/usr/lib/ make install && make clean
USER 1000

WORKDIR /home/ubuntu/ctest
