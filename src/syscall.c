#include <stdint.h>
#include <syscall.h>
#include <isr.h>

#include <monitor.h>
#include <fs.h>
#include <task.h>
#include <elf.h>

static void syscall_handler(registers_t *regs);

static void *syscalls[9] =
{
   &monitor_write,
   &monitor_write_hex,
   &monitor_write_dec,
   &monitor_put,
   &fork,
   &getpid,
   &read_fs,
   &finddir_fs,
   &readdir_fs
};
uint32_t num_syscalls = 9;

void initialise_syscalls()
{
   // Register our syscall handler.
   register_interrupt_handler (0x80, &syscall_handler);
}

void syscall_handler(registers_t *regs)
{
   // Firstly, check if the requested syscall number is valid.
   // The syscall number is found in EAX.
   if (regs->eax >= num_syscalls)
       return;

   // Get the required syscall location.
   void *location = syscalls[regs->eax];

   // We don't know how many parameters the function wants, so we just
   // push them all onto the stack in the correct order. The function will
   // use all the parameters it wants, and we can pop them all back off afterwards.
   int ret;
   asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs->edi), "r" (regs->esi), "r" (regs->edx), "r" (regs->ecx), "r" (regs->ebx), "r" (location));
   regs->eax = ret;
}

DEFN_SYSCALL1(monitor_write, 0, const char*);
DEFN_SYSCALL1(monitor_write_hex, 1, const char*);
DEFN_SYSCALL1(monitor_write_dec, 2, const char*);
DEFN_SYSCALL1(monitor_put, 3, char);

DEFN_SYSCALL0(fork, 4)
DEFN_SYSCALL0(getpid, 5)

DEFN_SYSCALL4(read_fs, 6, fs_node_t* , uint32_t, uint32_t, uint8_t*)
DEFN_SYSCALL2(finddir_fs, 7, fs_node_t*, char)
DEFN_SYSCALL2(readdir_fs, 8, fs_node_t*, uint32_t)