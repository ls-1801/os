#include "mmu.h"
#include "../lib/bits.h"

struct L1Mapping {
  uint32_t mappings[4096];
};

#define PA_BIT 10
#define XN_BIT 4
#define PXN_BIT 0
#define SCTLR_CACHE_BIT 2
#define SCTLR_ICACHE_BIT 12
#define SCTLR_MMU_BIT 0

const char *get_section(uint32_t ptr) {
  if (ptr < 0x100000) {
    return "NULL Segment";
  } else if (ptr < 0x200000) {
    return "Kernel Code";
  } else if (ptr < 0x300000) {
    return "Kernel ReadOnly Data";
  } else if (ptr < 0x400000) {
    return "Kernel Data";
  } else if (ptr < 0x500000) {
    return "Kernel Stack";
  } else if (ptr < 0x600000) {
    return "User Stack";
  } else if (ptr < 0x700000) {
    return "User Code";
  } else if (ptr < 0x800000) {
    return "User ReadOnly Data";
  } else if (ptr > 0x7E200000 && ptr < 0x7E300000) {
    return "Near Uart";
  } else if (ptr > 0x7E000000 && ptr < 0x7E100000) {
    return "Near IR or Timer";
  } else {
    return "Unassigned";
  }
}

void __set_sctlr(uint32_t sctlr) {
  asm volatile("MCR p15, 0, %0, c1, c0, 0" ::"r"(sctlr));
}

uint32_t __get_sctlr() {
  uint32_t sctlr;
  asm volatile("MRC p15, 0, %0, c1, c0, 0" : "=r"(sctlr));
  return sctlr;
}

void __set_ttbr0(uint32_t ttbr0) {
  asm volatile("MCR p15, 0, %0, c2, c0, 0" ::"r"(ttbr0));
}

uint32_t __get_ttbr0() {
  uint32_t ttbr0;
  asm volatile("MRC p15, 0, %0, c2, c0, 0" : "=r"(ttbr0));
  return ttbr0;
}

void __set_ttbcr(uint32_t ttbcr) {
  asm volatile("MCR p15, 0, %0, c2, c0, 2" ::"r"(ttbcr));
}

uint32_t __get_ttbcr() {
  uint32_t ttbcr;
  asm volatile("MRC p15, 0, %0, c2, c0, 2" : "=r"(ttbcr));
  return ttbcr;
}

void __set_dacr(uint32_t dacr) {
  asm volatile("MCR p15, 0, %0, c3, c0, 0" ::"r"(dacr));
}

uint32_t __get_dacr() {
  uint32_t dacr;
  asm volatile("MRC p15, 0, %0, c3, c0, 0" : "=r"(dacr));
  return dacr;
}

// AP[2] + TEX (always 0) + AP[2:0]
enum PageAccess {
  no_access = 0b000000,     /*0b0 000 00*/
  sys_access = 0b000001,    /*0b0 000 01*/
  sys_ro_access = 0b100001, /*0b1 000 01*/
  ro_access = 0b000010,     /*0b0 000 10*/
  full_access = 0b000011,   /*0b0 000 11*/
};

// Align mapping to 2^14 byte.
static struct L1Mapping default_mapping __attribute__((__aligned__(16384)));

void set_mapping(struct L1Mapping *mapping, void *virt, void *phys,
                 enum PageAccess pa, bool XN, bool PXN) {
  /**
   * Use top 10 bits of the virtual address as an index into the l1 mapping.
   * Use the top 10 bits of the physical address.
   * 19 NS - 0
   * 18 - 0
   * 17 nG - 0
   * 16 s - 0
   * [15:10] - PageAccess
   * 9 IMPL? - 0
   * [8:5] domain - 0 (client domain)
   * 4 XN - XN
   * 3 C - 0
   * 2 B - 0
   * 1 - 1
   * 0 PXN - PXN
   **/
  uint32_t mapping_entry = ((uint32_t)phys & 0xFFF00000) | (pa << PA_BIT) |
                           (XN << XN_BIT) | (PXN << PXN_BIT) | 1 << 1;
  mapping->mappings[(uint32_t)virt >> 20] = mapping_entry;
  // kprintf("index: %i: %x\t (%x -> %x)\n", (uint32_t)virt >> 20,
  // mapping_entry,
  //         ((uint32_t)virt & 0xFFF00000), ((uint32_t)phys & 0xFFF00000));
}

void create_default_mapping(struct L1Mapping *mapping) {
  int i;
  for (i = 0; i < 4096; i++) {
    set_mapping(mapping, ((void *)(i << 20)), NULL, no_access, true, true);
  }
}

void init_mmu() {
  create_default_mapping(&default_mapping);

  // Page Also Contains initialization Code which has already been executed and
  // should never be run again.
  set_mapping(&default_mapping, (uint32_t *)(0x000000), (uint32_t *)(0x000000),
              no_access, true, true);

  // Kernel Code (SysReadOnly, Kernel Execution only)
  set_mapping(&default_mapping, (uint32_t *)(0x100000), (uint32_t *)(0x100000),
              sys_ro_access, false, false);

  // Kernel RoData (SysReadOnly, No Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x200000), (uint32_t *)(0x200000),
              sys_ro_access, true, true);

  // Kernel Data BSS (Sys, No Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x300000), (uint32_t *)(0x300000),
              sys_access, true, true);

  // Kernel Stacks (Sys, No Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x400000), (uint32_t *)(0x400000),
              sys_access, true, true);

  // User Stack (Full Access, No Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x500000), (uint32_t *)(0x500000),
              full_access, true, true);

  // User Code (ReadOnly, User Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x600000), (uint32_t *)(0x600000),
              ro_access, false, true);

  // User RoData (ReadOnly, No Execution)
  set_mapping(&default_mapping, (uint32_t *)(0x700000), (uint32_t *)(0x900000),
              ro_access, true, true);

  // UART MAPPING
  set_mapping(&default_mapping, (uint32_t *)(0x7E200000),
              (uint32_t *)(0x7E200000 - 0x3F000000), sys_access, true, true);

  // TIMER / IR MAPPING
  set_mapping(&default_mapping, (uint32_t *)(0x7E000000),
              (uint32_t *)(0x7E000000 - 0x3F000000), sys_access, true, true);

  // Set L1 Table.
  // default_mapping is aligned to 2^14 bytes.
  // only the [31:14] matters rest is 0.
  __set_ttbr0((uint32_t)&default_mapping);

  uint32_t dacr = __get_dacr();
  // Set Domain 0 to Client
  dacr = (dacr & 0xFFFFFFFC) | 0b01;
  __set_dacr(dacr);

  // 32-Bit Pages
  __set_ttbcr(0);

  uint32_t sctlr = __get_sctlr();
  // Disable Caches
  UNSET_BIT(sctlr, SCTLR_CACHE_BIT);
  UNSET_BIT(sctlr, SCTLR_ICACHE_BIT);
  // Enable MMU
  SET_BIT(sctlr, SCTLR_MMU_BIT);
  __set_sctlr(sctlr);
}