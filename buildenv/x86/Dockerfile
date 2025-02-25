FROM randomdude/gcc-cross-i686-elf

RUN apt-get update
RUN apt upgrade -y
RUN apt install -y nasm xorriso grub-pc-bin grub-common nano

VOLUME /root/env
WORKDIR /root/env
