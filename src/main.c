// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
//           Made for JamesM's tutorials <www.jamesmolloy.co.uk>

#include <stdint.h>
#include <monitor.h>
#include <descriptor_tables.h>
#include <timer.h>
#include <paging.h>
#include <multiboot.h>
#include <fs.h>
#include <initrd.h>
#include <task.h>
#include <syscall.h>

extern uint32_t placement_address;
uint32_t initial_esp;

extern void (switch_to_user_mode)(void);

int kernel_main(multiboot_info_t *mboot_ptr, uint32_t initial_stack)
{
    initial_esp = initial_stack;
    // Initialise all the ISRs and segmentation
    init_descriptor_tables();
    // Initialise the screen (by clearing it)
    monitor_clear();

    // Initialise the PIT to 100Hz
    asm volatile("sti");
    init_timer(50);

    // Find the location of our initial ramdisk.
    ASSERT(mboot_ptr->mods_count > 0);
    uint32_t initrd_location = *((uint32_t*)mboot_ptr->mods_addr);
    uint32_t initrd_end = *(uint32_t*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    // Initialise the initial ramdisk, and set it as the filesystem root.
    fs_root = initialise_initrd(initrd_location);

    initialise_syscalls();

    switch_to_user_mode();

    syscall_monitor_write("Hello, user world!\n");

    return 0;
}
