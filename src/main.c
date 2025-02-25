// main.c -- Defines the C-code kernel entry point, calls initialisation routines.
// Made for JamesM's tutorials
#include "monitor.h"
#include "isr.h"

int main(struct multiboot *mboot_ptr)
{

  monitor_clear();
  monitor_write("Hello, world!\n");

  init_descriptor_tables();

  asm volatile ("int $0x3");
  asm volatile ("int $0x4");
  // All our initialisation calls will go in here.
  return 0xDEADBABA;
}
