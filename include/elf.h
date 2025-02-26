#ifndef ELF_H
#define ELF_H

#include <stdint.h>

struct elf_header
{
   uint8_t magic[4]; // ELF magic
   uint8_t mode; // 1 = 32 bit, 2 = 64 bit
   uint8_t endianness; // 1 = little endian, 2  = big endian
   uint8_t header_version; // ELF header version
   uint8_t os_abi; // OS ABI - usually 0 for System-V
   uint8_t unused[8];
   uint16_t type; // 1 = relocatale, 2 = executable, 3 = shared, 4 = core
   uint16_t instruction_set; // see comment below
   uint32_t elf_version; // ELF version (currently 1)
   uint32_t entry_offset; // Program entry offset
   uint32_t header_table_offset; // Program header table offset
   uint32_t section_header_table_offset; // Section Header table offset
   uint32_t flags; //architecture dependent
   uint16_t entry_size; // Size of an entry in program header table
   uint16_t prog_header_entries; // Number of entries in the program header table
   uint16_t prog_entry_size; // Size of an entry in the section header table
   uint16_t num_section_entries; // Number of entries in the section header table
   uint16_t section_index; // Section index to the section header string table

} __attribute__((packed));
typedef struct elf_header elf_header_t;

// ISAs can be found at https://wiki.osdev.org/ELF
// We are using x86, so the value should be 0x03

struct elf_program_header
{
    uint32_t segment_type; // type of segment
    uint32_t p_offset; // the offset in the file that the data for this segment can be found
    uint32_t p_vaddr; // where you should start to put this segment in virtual memory
    uint32_t p_paddr; // Reserved for segment's physical address
    uint32_t p_filesz; // Size of the segment in the file
    uint32_t p_memsz; // Size of the segment in memory
    uint32_t flags; // Flags
    uint32_t alignment; // The required alignment for this section (usually a power of 2)
} __attribute__((packed));
typedef struct elf_program_header elf_program_header_t;

//Segment types: 
// 0 = null - ignore the entry;
// 1 = load - clear p_memsz bytes at p_vaddr to 0, then copy p_filesz bytes from p_offset to p_vaddr;
// 2 = dynamic - requires dynamic linking; 3 = interp - contains a file path to an executable to use as an interpreter for the following segment;
// 4 = note section. There are more values, but mostly contain architecture/environment specific information, which is probably not required for the majority of ELF files.


// Flags:
// 1 = executable,
// 2 = writable,
// 4 = readable.

int execfile(char * data);

#endif
