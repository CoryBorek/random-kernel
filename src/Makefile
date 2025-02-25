# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o main.o monitor.o common.o descriptor_tables.o isr.o interrupt.o gdt.o timer.o \
	kheap.o paging.o ordered_array.o fs.o initrd.o task.o process.o

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -ffreestanding
LDFLAGS=-Tlink.ld -m elf_i386
ASFLAGS=-felf

.PHONY: all clean

all: myos.iso

make_initrd: initrdgen.c
	$(CC) -Wall initrdgen.c -o make_initrd

clean:
	rm -f *.o kernel
	rm -f test.txt test2.txt make_initrd
	rm -f myos.iso

kernel: $(SOURCES)
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

myos.iso: kernel make_initrd
	mkdir -p isodir/boot/grub
	cp kernel isodir/boot/kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	echo "test" > test.txt
	echo "test2" > test2.txt
	./make_initrd test.txt test.txt test2.txt test2.txt
	cp initrd.img isodir/boot/initrd
	grub-mkrescue -o myos.iso isodir
	rm -r isodir
