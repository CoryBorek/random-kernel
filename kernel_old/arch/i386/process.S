.global read_eip
read_eip: 
    popl %eax                   # Get the return address
    jmp *%eax                   # Return. Can't use RET because return
                                # address popped off the stack. 

.global copy_page_physical
copy_page_physical: 
    pushl %ebx            # According to __cdecl, we must preserve the contents of EBX.
    pushf                 # push EFLAGS, so we can pop it and reenable interrupts
                          # later, if they were enabled anyway.
    cli                   # Disable interrupts, so we aren't interrupted.
                          # Load these in BEFORE we disable paging!
    movl 12(%esp),%ebx    # Source address
    movl 16(%esp),%ecx    # Destination address

    movl %cr0, %edx       # Get the control register...
    andl $0x7fffffff,%edx # and...
    movl %edx, %cr0       # Disable paging.

    movl $1024,%edx       # 1024*4bytes = 4096 bytes

.loop: 
    movl (%ebx),%eax      # Get the word at the source address
    movl %eax,(%ecx)      # Store it at the dest address
    addl $4,%ebx          # Source address += sizeof(word)
    addl $4,%ecx          # Dest address += sizeof(word)
    decl %edx             # One less word to do
    jnz .loop

    movl %cr0, %edx       # Get the control register again
    orl $0x80000000,%edx  # and...
    movl %edx, %cr0       # Enable paging.

    popf                  # Pop EFLAGS back.
    popl %ebx             # Get the original value of EBX back.
    ret


# Here we:
# * Stop interrupts so we don't get interrupted.
# * Temporarily put the new EIP location in ECX.
# * Temporarily put the new page directory's physical address in EAX.
# * Set the base and stack pointers
# * Set the page directory
# * Put a dummy value (0x12345) in EAX so that above we can recognize that we've just
#   switched task.
# * Restart interrupts. The STI instruction has a delay - it doesn't take effect until after
#   the next instruction.
# * Jump to the location in ECX (remember we put the new EIP in there).

.global perform_task_switch
perform_task_switch: 
     cli #
     movl 4(%esp),%ecx  # EIP
     movl 8(%esp),%eax  # physical address of current directory
     movl 12(%esp),%ebp # EBP
     movl 16(%esp),%esp # ESP
     movl %eax, %cr3    # set the page directory
     movl $0x12345,%eax # magic number to detect a task switch
     sti #
     jmp *%ecx

