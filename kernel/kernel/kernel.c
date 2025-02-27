#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/init.h>
#include <kernel/syscall.h>

void kernel_main(multiboot_info_t *mboot_ptr, uint32_t initial_stack) {
	terminal_initialize();
	init_systems(mboot_ptr, initial_stack);
	syscall_monitor_write("Hello, kernel World!\n");
}
