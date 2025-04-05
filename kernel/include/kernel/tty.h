#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void console_init(void);
void console_putchar(char c);
void console_write(const char* data, size_t size);
void console_writestring(const char* data);

#endif
