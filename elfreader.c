#include "elfreader.h"

struct elf_info* get_elf_info(uint8_t* image) {
  struct elf_info* elf = (struct elf_info*)malloc(sizeof(struct elf_info));
  elf->bitwidth = get_elf_bitwidth(image);
  elf->endianness = get_elf_endianness(image);
  elf->type = get_elf_type(image);
  elf->machine = get_elf_machine(image);
  elf->entry = get_elf_entry(image);
  elf->abi = get_elf_abi(image);
  return elf;
}

int print_elf_info(struct elf_info* elf) {
  const char* endian_str = elf->endianness == LITTLEENDIAN ? "little-endian" : "big-endian";
  char* type_str;
  switch(elf->type) {
    case REL:
      type_str = "Relocatable file";
      break;
    case EXEC:
      type_str = "Executable file";
      break;
    case DYN:
      type_str = "Shared object file";
      break;
    default:
      type_str = "Core";
      break;
  }
  
  printf("bitwidth: %d\nendianness: %s\ntype: %s\nmachine: %s\nentry: %p\nABI: %s\n\n", 
         elf->bitwidth, 
         endian_str,
         type_str,
         machines[elf->machine],
         elf->entry,
         abis[elf->abi]);

  return 0;
}

uint8_t* read_elf(const char* path) {
  struct stat statbuffer;
  int status;

  status = stat(path, &statbuffer);
  if(status != 0) {
    printf("Unable to open %s\n", path);
    return NULL;
  }

  FILE* fileptr;
  uint8_t* filebuffer = (uint8_t*)malloc(sizeof(uint8_t) * statbuffer.st_size);
  size_t r;

  fileptr = fopen(path, "rb");
  if(fileptr == NULL) {
    printf("Unable to open %s\n", path);
    return NULL;
  }

  while(!feof(fileptr)) {
    fread(filebuffer, statbuffer.st_size, 1, fileptr);
  }

  fclose(fileptr);
  return filebuffer;
}

uint8_t get_elf_machine(uint8_t* image) {
  return image[ELF_MACHINE];
}

enum ELFTYPE get_elf_type(uint8_t* image) {
  if(image[ELF_TYPE] <= 0x04) {
    return image[ELF_TYPE];
  }
  return NOTYPE;
}


uint64_t get_elf_entry(uint8_t* image) {
  if(image[ELF_BITWIDTH] != 0x01) {
    return *((uint64_t*)(image + ELF_ENTRY));
  }
  return *((uint32_t*)(image + ELF_ENTRY));
}

enum ELFENDIANNESS get_elf_endianness(uint8_t* image) {
  return image[ELF_ENDIANNESS] == 0x01 ? LITTLEENDIAN : BIGENDIAN;
}

uint8_t get_elf_bitwidth(uint8_t* image) {
  return image[ELF_BITWIDTH] == 0x01 ? 32 : 64;
}

uint8_t get_elf_abi(uint8_t* image) {
  return image[ELF_ABI];
}

const char* machines[95] = {"e_machine"
, "AT&T WE 32100"
, "Sun SPARC"
, "Intel 80386"
, "Motorola 68000"
, "Motorola 88000"
, "Intel 80486"
, "Intel i860"
, "MIPS RS3000 Big-Endian"
, "IBM System/370 Processor"
, "MIPS RS3000 Little-Endian"
, "RS6000"
, "Unknown"
, "Unknown"
, "Unknown"
, "PA-RISC"
, "nCUBE"
, "Fujitsu VPP500"
, "Sun SPARC 32+"
, "Intel 80960"
, "PowerPC"
, "64-bit PowerPC"
, "IBM System/390 Processor"
, "Unknown" 
, "Unknown" 
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "Unknown"
, "NEX V800"
, "Fujitsu FR20"
, "TRW RH-32"
, "Motorola RCE"
, "Advanced RISC Marchines ARM"
, "Digital Alpha"
, "Hitachi SH"
, "Sun SPARC V9 (64-bit)"
, "Siemens Tricore embedded processor"
, "Argonaut RISC Core,"
, "Hitachi H8/300"
, "Hitachi H8/300H"
, "Hitachi H8S"
, "Hitachi H8/500"
, "Intel IA64"
, "Stanford MIPS-X"
, "Motorola ColdFire"
, "Motorola M68HC12"
, "Fujitsu MMA Mulimedia Accelerator"
, "Siemens PCP"
, "Sony nCPU embedded RISC processor"
, "Denso NDR1 microprocessor"
, "Motorola Star*Core processor"
, "Toyota ME16 processor"
, "STMicroelectronics ST100 processor"
, "Advanced Logic Corp. TinyJ"
, "AMDs x86-64 architecture"
, "Sony DSP Processor"
, "Unknown"
, "Unknown"
, "Siemens FX66 microcontroller"
, "STMicroelectronics ST9+8/16 bit"
, "STMicroelectronics ST7 8-bit"
, "Motorola MC68HC16 Microcontroller"
, "Motorola MC68HC11 Microcontroller"
, "Motorola MC68HC08 Microcontroller"
, "Motorola MC68HC05 Microcontroller"
, "Silicon Graphics SVx"
, "STMicroelectronics ST19 8-bit"
, "Digital VAX"
, "Axis Communications 32-bit"
, "Infineon Technologies 32-bit"
, "Element 14 64-bit DSP Processor"
, "LSI Logic 16-bit DSP Processor"
, "Donald Knuth's educational"
, "Harvard University"
, "SiTera Prism"
, "Atmel AVR 8-bit microcontroller"
, "Fujitsu FR30"
, "Mitsubishi D10V"
, "Mitsubishi D30V"
, "NEC v850"
, "Mitsubishi M32R"
, "Matsushita MN10300"
, "Matsushita MN10200"
, "picoJava"
, "OpenRISC 32-bit embedded processor"
, "ARC Cores Tangent-A5"
, "Tensilica Xtensa architecture" };


const char* abis[0x12] = {"System V", "HP-UX", "NETBSD", 
  "Linux", "GNU Hard", "Solaris", 
  "AIX", "IRIX", "FreeBSD", 
  "Tru64", "Novell Modesto", 
  "OpenBSD", "OpenVMS", "NonStop Kernel", 
  "AROS", "FenixOS", "Nuxi CloudABI", 
  "Stratus Technologies OpenVOS"};
