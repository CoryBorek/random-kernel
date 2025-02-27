#ifndef SYSCALL_H
#define SYSCALL_H

#include <kernel/fs.h>
#include <stdint.h>

void initialise_syscalls();

#define DECL_SYSCALL0(fn) int syscall_##fn();
#define DECL_SYSCALL1(fn,p1) int syscall_##fn(p1);
#define DECL_SYSCALL2(fn,p1,p2) int syscall_##fn(p1,p2);
#define DECL_SYSCALL3(fn,p1,p2,p3) int syscall_##fn(p1,p2,p3);
#define DECL_SYSCALL4(fn,p1,p2,p3,p4) int syscall_##fn(p1,p2,p3,p4);
#define DECL_SYSCALL5(fn,p1,p2,p3,p4,p5) int syscall_##fn(p1,p2,p3,p4,p5);


DECL_SYSCALL1(monitor_write, const char*)
DECL_SYSCALL1(monitor_write_hex, uint32_t)
DECL_SYSCALL1(monitor_write_dec, uint32_t)
DECL_SYSCALL1(monitor_put, char)

DECL_SYSCALL0(fork)
DECL_SYSCALL0(getpid)

DECL_SYSCALL4(read_fs, fs_node_t* , uint32_t, uint32_t, uint8_t*)
DECL_SYSCALL2(finddir_fs, fs_node_t*, char)
DECL_SYSCALL2(readdir_fs, fs_node_t*, uint32_t)

#endif