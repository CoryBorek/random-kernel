#ifndef INIT_H
#define INIT_H

#include <stdint.h>
#include <kernel/multiboot.h>

void init_systems(multiboot_info_t *mboot_ptr, uint32_t initial_stack);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, uint32_t line);
extern void panic_assert(const char *file, uint32_t line, const char *desc);


#endif