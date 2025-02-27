#include <kernel/init.h>
#include <kernel/multiboot.h>

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <kernel/fs.h>

#include "descriptor_tables.h"
#include "timer.h"
#include "paging.h"
#include "task.h"
#include <kernel/initrd.h>
#include <kernel/syscall.h>



extern uint32_t placement_address;
uint32_t initial_esp;

void init_systems(multiboot_info_t *mboot_ptr, uint32_t initial_stack) {
    initial_esp = initial_stack;
    init_descriptor_tables();

    asm volatile("sti");
    init_timer(50);

    // Find the location of our initial ramdisk.
    
    //TODO: re-add initrd
    ASSERT(mboot_ptr->mods_count > 0);
    uint32_t initrd_location = *((uint32_t*)mboot_ptr->mods_addr);
    uint32_t initrd_end = *(uint32_t*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

    // Start paging.
    initialise_paging();

    // Start multitasking.
    initialise_tasking();

    fs_root = initialise_initrd(initrd_location);

    // Create a new process in a new address space which is a clone of this.
   int ret = fork();

   printf("fork() returned %x, and getpid() returned %x\n", ret, getpid());
   printf("============================================================================\n");

   // The next section of code is not reentrant so make sure we aren't interrupted during.
   asm volatile("cli");
   // list the contents of /
   int i = 0;
   struct dirent *node = 0;

   char buf[512] = {0};

   while ( (node = readdir_fs(fs_root, i)) != 0)
   {   
       printf("Found file %s\n", node->name);
       fs_node_t *fsnode = finddir_fs(fs_root, node->name);
       if (strcmp(node->name, "test.elf") == 0) {

        printf("flags: 0x%x\n", fsnode->flags);
        uint32_t sz = read_fs(fsnode, 0, 512, buf);
        printf("size: 0x%x\n", sz);
       }
       i++;
   }

   asm volatile("sti");

   initialise_syscalls();
   switch_to_user_mode();

   if (strlen(buf) > 0) {
        int test = execfile(buf);
        syscall_monitor_write("test: ");
        syscall_monitor_write_dec(test);
        syscall_monitor_write("\n");
    }

   syscall_monitor_write("Hello user world!\n");
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