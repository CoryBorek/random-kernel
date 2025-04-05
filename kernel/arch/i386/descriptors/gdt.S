.global gdt_flush
gdt_flush: 
   movl 4(%esp),%eax # Get the pointer to the GDT, passed as a parameter.
   lgdt (%eax)

   movw $0x10,%ax    # 0x10 is the offset in the GDT to our data segment
   movw %ax,%ds      # Load all data segment selectors
   movw %ax,%es
   movw %ax,%fs
   movw %ax,%gs
   movw %ax,%ss
   jmp $0x08,$.flush   # 0x08 is the offset to our code segment: Far jump!
.flush: 
   ret

.global idt_flush

idt_flush: 
   movl 4(%esp),%eax # Get the pointer to the IDT, passed as a parameter.
   lidt (%eax)
   ret

.global tss_flush
tss_flush: 
   movw $0x2B,%ax    # Load the index of our TSS structure - The index is
                     # 0x28, as it is the 5th selector and each is 8 bytes
                     # long, but we set the bottom two bits (making 0x2B)
                     # so that it has an RPL of 3, not zero.
   ltrw %ax          # Load 0x2B into the task state register.
   ret

