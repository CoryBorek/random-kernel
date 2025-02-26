# Makefile for JamesM's kernel tutorials.
# The C and C++ rules are already setup by default.
# The only one that needs changing is the assembler
# rule, as we use nasm instead of GNU as.

CC=i686-elf-gcc
BASEGCC=gcc
AS=nasm
LD=ld

OBJS= obj/boot.o \
		 obj/main.o \
		 obj/monitor.o \
		 obj/common.o \
		 obj/descriptor_tables.o \
		 obj/isr.o \
		 obj/interrupt.o \
		 obj/gdt.o \
		 obj/timer.o \
		 obj/kheap.o \
		 obj/paging.o \
		 obj/ordered_array.o \
		 obj/fs.o \
		 obj/initrd.o \
		 obj/task.o \
		 obj/process.o \
		 obj/syscall.o \
		 obj/elf.o \


CFLAGS=-nostdlib -lgcc -fno-builtin -fno-stack-protector -m32 -ffreestanding -Wall -Wextra -Iinclude
#-std=gnu99 -ffreestanding -O2 -Wall -Wextra -m32

LDFLAGS=-Tlink.ld -m elf_i386 -Iinclude
ASFLAGS=-felf

.PHONY: all clean run

all: myos.iso

obj/%.o: src/%.c
	mkdir -p obj
	$(CC) $(CFLAGS) -o $@ -c $<

obj/%.o: src/%.s
	mkdir -p obj
	$(AS) $(ASFLAGS) -o $@ $<

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o kernel $(OBJS)

make_initrd: tools/initrdgen.c
	$(BASEGCC) -Wall tools/initrdgen.c -o make_initrd

test.elf: tools/test.c
	$(BASEGCC) -Wall $< -o $@

clean:
	rm -rf obj
	rm -f kernel myos.iso initrd.img make_initrd
	rm -f test*.txt
	rm -f test.elf


myos.iso: kernel make_initrd test.elf
	mkdir -p isodir/boot/grub
	cp kernel isodir/boot/kernel
	cp grub.cfg isodir/boot/grub/grub.cfg
	echo "test" > test.txt
	echo "test2" > test2.txt
	./make_initrd test.txt test.txt test.elf test.elf
	cp initrd.img isodir/boot/initrd
	grub-mkrescue -o myos.iso isodir
	rm -r isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso