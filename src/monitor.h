// monitor.h -- Defines the interface for monitor.h
// From JamesM's kernel development tutorials.

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen.
void monitor_put(char c);

// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);

void monitor_write_hex(u32int n);

void monitor_write_dec(u32int n);

#define PANIC(x) do { \
    monitor_write("PANIC: "); \
    monitor_write(x);         \
    monitor_write("!\n");     \
    for(;;);                  \
} while (0)

#endif // MONITOR_H
