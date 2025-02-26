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

       // Create a new process in a new address space which is a clone of this.
   int ret = fork();

   monitor_write("fork() returned ");
   monitor_write_hex(ret);
   monitor_write(", and getpid() returned ");
   monitor_write_hex(getpid());
   monitor_write("\n============================================================================\n");

   // The next section of code is not reentrant so make sure we aren't interrupted during.
   asm volatile("cli");
   // list the contents of /
   int i = 0;
   struct dirent *node = 0;

   char buf[512];

   while ( (node = readdir_fs(fs_root, i)) != 0)
   {
       monitor_write("Found file ");
       monitor_write(node->name);
       monitor_write("\n");
       fs_node_t *fsnode = finddir_fs(fs_root, node->name);
       if (strcmp(node->name, "test.elf") == 0) {
        monitor_write("flags: ");
        monitor_write_hex(fsnode->flags);
        monitor_write("\n");
        uint32_t sz = read_fs(fsnode, 0, 512, buf);
        monitor_write("size: ");
        monitor_write_dec(sz);
        monitor_write("\n");
       }
       i++;
   }

   asm volatile("sti");

    initialise_syscalls();
    switch_to_user_mode();

    int test = execfile(buf);
   syscall_monitor_write("test: ");
   syscall_monitor_write_dec(test);
   syscall_monitor_write("\n");

   syscall_monitor_write("Hello user world!\n");
   
    return 0;
}
