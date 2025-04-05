#include <kernel/init.h>
#include <kernel/multiboot.h>
#include <kernel/defs.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "descriptor_tables.h"
#include "i386.h"
#include "timer.h"

extern uint32_t placement_address;
uint32_t initial_esp;


void init_arch(multiboot_info_t *mboot_ptr, uint32_t initial_stack) {
    printf("\n==Initializing i386 System==\n");
    initial_esp = initial_stack;
    init_descriptor_tables();

    sti();
    init_timer(50);
}

extern void panic(const char *message, const char *file, uint32_t line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    printf("PANIC(%s) at %s:%d\n", message, file, line);
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, uint32_t line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    printf("ASSERTION-FAILED(%s) at %s:%d\n", desc, file, line);
    // Halt by going into an infinite loop.
    for(;;);
}