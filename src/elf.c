#include <elf.h>
#include <stdint.h>
#include <syscall.h>

int execfile(char * data) {

    
    if (!(data[0] == 0x7F && data[1] == 'E' && data[2] == 'L' && data[3] == 'F')) {
        syscall_monitor_write("cannot run file that is not an ELF binary.\n");
    }

    elf_header_t header;

    memcpy((uint8_t*)&header, data, sizeof(elf_header_t));

    syscall_monitor_write("ELF Data: \n");
    syscall_monitor_write("Elf version: ");
    syscall_monitor_write_dec(header.elf_version);
    syscall_monitor_write("\nInstruction set: ");
    syscall_monitor_write_hex(header.instruction_set);
    syscall_monitor_write("\n");
    if (!(header.elf_version == 1 && header.instruction_set == 0x03)) {
        syscall_monitor_write("This computer is invalid for running this program.\n");
    }

    uint8_t * program_header_table_loc = data + header.header_table_offset;

    uint16_t num_program_header_entries = header.prog_header_entries;

    uint16_t num_section_header_entries = header.num_section_entries;

    syscall_monitor_write("Program header table entries: ");
    syscall_monitor_write_dec(num_program_header_entries);
    syscall_monitor_write("\n");

    syscall_monitor_write("Section header table entries: ");
    syscall_monitor_write_dec(num_section_header_entries);
    syscall_monitor_write("\n");

}