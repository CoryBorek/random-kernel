#ifndef __DEFS_H
#define __DEFS_H

#include <stddef.h>
#include <stdint.h>

// === Interrupts ===
int enable_interrupts();
int disable_interrupts();

// === TASKING ===

// Initialises the tasking system.
void init_tasking();

// Forks the current process, spawning a new one with a different
// memory space.
int fork();

// Causes the current process' stack to be forcibly moved to a new location.
void move_stack(void *new_stack_start, uint32_t size);

// Returns the pid of the current process.
int getpid();


// === PAGING ===

/**
   Sets up the environment, page directories etc and
   enables paging.
**/
void init_paging();

// === MEMORY ===
/**
   Allocate a chunk of memory, sz in size. If align == 1,
   the chunk must be page-aligned. If phys != 0, the physical
   location of the allocated chunk will be stored into phys.

   This is the internal version of kmalloc. More user-friendly
   parameter representations are available in kmalloc, kmalloc_a,
   kmalloc_ap, kmalloc_p.
**/
uint32_t kmalloc_int(size_t sz, int align, uint32_t *phys);

/**
   Allocate a chunk of memory, sz in size. The chunk must be
   page aligned.
**/
uint32_t kmalloc_a(size_t sz);

/**
   Allocate a chunk of memory, sz in size. The physical address
   is returned in phys. Phys MUST be a valid pointer to uint32_t!
**/
uint32_t kmalloc_p(size_t sz, uint32_t *phys);

/**
   Allocate a chunk of memory, sz in size. The physical address 
   is returned in phys. It must be page-aligned.
**/
uint32_t kmalloc_ap(size_t sz, uint32_t *phys);

/**
   General allocation function.
**/
uint32_t kmalloc(size_t sz);

/**
   General deallocation function.
**/
void kfree(void *p);

#endif