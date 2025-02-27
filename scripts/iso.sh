#!/bin/sh
set -e
. ./scripts/build.sh

gcc -Wall tools/initrdgen.c -o initrdgen
echo "test" > test.txt
echo "test2" > test2.txt
./initrdgen test.txt test.txt test2.txt test2.txt
rm -f test.txt test2.txt initrdgen test.elf

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

mv initrd.img isodir/boot/initrd
cp sysroot/boot/myos.kernel isodir/boot/myos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "myos" {
	multiboot /boot/myos.kernel
	module /boot/initrd
}
EOF
grub-mkrescue -o myos.iso isodir