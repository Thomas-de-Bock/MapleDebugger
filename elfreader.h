#include<stdio.h>
#include<stdlib.h>
#include<sys/ptrace.h>
#include<sys/procfs.h>
#include<sys/msg.h>
#include<sys/user.h>
#include<sys/wait.h>
#include<unistd.h>
#include <stdint.h>
#include <sys/ptrace.h>
#include<stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

enum ELFENDIANNESS {
  BIGENDIAN,
  LITTLEENDIAN
};

enum ELFTYPE {
  NOTYPE,
  REL,
  EXEC,
  DYN,
  CORE,
};

struct elf_info {
  uint8_t bitwidth;
  uint8_t machine;
  enum ELFENDIANNESS endianness;
  enum ELFTYPE type;
  uint64_t entry;
  uint8_t abi;
};

extern const char* machines[95];
extern const char* abis[0x12];

#define ELF_BITWIDTH 0x04
#define ELF_ENDIANNESS 0x05
#define ELF_ABI 0x07
#define ELF_TYPE 0x10
#define ELF_MACHINE 0x12
#define ELF_ENTRY 0x18

struct elf_info* get_elf_info(uint8_t* image);
int print_elf_info(struct elf_info* elf);
uint8_t* read_elf(const char* path);
uint8_t get_elf_machine(uint8_t* image);
enum ELFTYPE get_elf_type(uint8_t* image);
uint64_t get_elf_entry(uint8_t* image);
enum ELFENDIANNESS get_elf_endianness(uint8_t* image);
uint8_t get_elf_bitwidth(uint8_t* image);
uint8_t get_elf_abi(uint8_t* image);
