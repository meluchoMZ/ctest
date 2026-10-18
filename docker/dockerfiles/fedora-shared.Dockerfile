FROM ctest-base:fedora

COPY . /tmp
WORKDIR /tmp

USER root
RUN make install && make clean
USER 1000

WORKDIR /home/fedora/ctest
