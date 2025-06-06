#ifndef TASK_H
#define TASK_H
#include <stdint.h>
#include "paging.h"

#define KERNEL_STACK_SIZE 2048       // Use a 2kb kernel stack.

// This structure defines a 'task' - a process.
typedef struct task
{
    int id;                // Process ID.
    uint32_t esp, ebp;       // Stack and base pointers.
    uint32_t eip;            // Instruction pointer.
    page_directory_t *page_directory; // Page directory.
    uint32_t kernel_stack;   // Kernel stack location.
    struct task *next;     // The next task in a linked list.
} task_t;

// Called by the timer hook, this changes the running process.
void switch_task();

#endif
