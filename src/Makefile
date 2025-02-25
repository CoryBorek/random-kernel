# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

SOURCES=boot.o main.o monitor.o common.o interrupt.o isr.o gdt.o descriptor_tables.o 

CFLAGS=-nostdlib -nostdinc -fno-builtin -fno-stack-protector -m32 -ffreestanding
LDFLAGS=-Tlink.ld -m elf_i386
ASFLAGS=-felf

.PHONY: all clean

all: myos.iso

clean:
	rm *.o kernel

kernel: $(SOURCES)
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $<

myos.iso: kernel
	mkdir -p isodir/boot/grub
	cp kernel isodir/boot/kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir
	rm -r isodir
