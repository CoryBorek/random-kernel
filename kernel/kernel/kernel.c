#include <stdio.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/multiboot.h>
#include <kernel/init.h>
#include <kernel/defs.h>
#include <kernel/fs.h>

extern uint32_t placement_address;

void kernel_main(multiboot_info_t *mboot_ptr, uint32_t initial_stack) {
	console_init();
	init_arch(mboot_ptr, initial_stack);



	//TODO: re-add initrd
    ASSERT(mboot_ptr->mods_count > 0);
    uint32_t initrd_location = *((uint32_t*)mboot_ptr->mods_addr);
    uint32_t initrd_end = *(uint32_t*)(mboot_ptr->mods_addr+4);
    // Don't trample our module with placement accesses, please!
    placement_address = initrd_end;

	init_paging();

	init_tasking();


	fs_root = initialise_initrd(initrd_location);

    // Create a new process in a new address space which is a clone of this.
   int ret = fork();


	printf("fork() returned %d, and getpid() returned %d\n", ret, getpid());
	printf("============================================================================\n");

	

disable_interrupts();
   int i = 0;
   struct dirent *node = 0;

   char buf[512] = {0};

   while ( (node = readdir_fs(fs_root, i)) != 0)
   {   
       printf("Found file %s\n", node->name);
       fs_node_t *fsnode = finddir_fs(fs_root, node->name);

        printf("flags: 0x%x\n", fsnode->flags);
        uint32_t sz = read_fs(fsnode, 0, 512, buf);
        printf("size: 0x%x\n", sz);
       i++;
   }

   enable_interrupts();

	if (ret == 0) {
		initialise_syscalls();
		switch_to_user_mode();
		syscall_enable_interrupts();

		int ret2 = syscall_fork();

		syscall_monitor_write("Hello user world (child)!\n\n");
		syscall_monitor_write("ret2: ");
		syscall_monitor_write_dec(ret2);
	} else {
		printf("Hello world!!\n");
	}
	while(1){}

}
