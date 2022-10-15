#ifndef DEBUG_UTIL_H
#define DEBUG_UTIL_H
#include "../lib/bits.h"
#include "../lib/io.h"
#include "system.h"
#include <stdint.h>

#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define SL 10
#define R11 11
#define FP 11
#define R12 12
#define IP 12
#define R15 13
#define PC 13

static inline void register_snapshot(uint32_t *regs) {
  kprintf("\n>>> Register snapshot (current mode) <<<\n");
  kprintf("R0:\t%x\tR8:\t%x\n", regs[R0], regs[R8]);
  kprintf("R1:\t%x\tR9:\t%x\n", regs[R1], regs[R9]);
  kprintf("R2:\t%x\tR10:\t%x\n", regs[R2], regs[R10]);
  kprintf("R3:\t%x\tR11:\t%x\n", regs[R3], regs[R11]);
  kprintf("R4:\t%x\tR12:\t%x\n", regs[R4], regs[R12]);
  kprintf("R5:\t%x\tSP:\t%x\n", regs[R5], __get_sp());
  kprintf("R6:\t%x\tLR:\t%x\n", regs[R6], __get_lr());
  kprintf("R7:\t%x\tPC:\t%x\n", regs[R7], regs[PC]);
}

static inline void print_bar() {
  kprintf(
      "\n####################################################################"
      "############\n");
}

static inline char *format_status_register(char *buf, int spsr) {
  return sprintf(
      buf, "%c%c%c%c %c %c%c%c %s(%x)", CHECK_BIT(spsr, 31) ? 'N' : '_',
      CHECK_BIT(spsr, 30) ? 'Z' : '_', CHECK_BIT(spsr, 29) ? 'C' : '_',
      CHECK_BIT(spsr, 28) ? 'V' : '_', CHECK_BIT(spsr, 9) ? 'E' : '_',
      CHECK_BIT(spsr, 7) ? 'I' : '_', CHECK_BIT(spsr, 6) ? 'F' : '_',
      CHECK_BIT(spsr, 8) ? 'A' : '_', get_mode_description(spsr), spsr);
}

static void status_register_snapshot(int cpsr, int spsr, enum arm_mode mode) {
  static enum arm_mode current_mode;
  static uint32_t spsrs[5];
  static uint32_t lrs[6];
  static uint32_t sps[6];

  current_mode = mode;
  /* MAKE SURE NONE OF THESE ARE PUT ON THE STACK !! */
  // change_irq_free_system_mode();
  change_mode(system_mode);
  lrs[0] = __get_lr();
  sps[0] = __get_sp();

  change_mode(svc_mode);
  spsrs[0] = __get_spsr();
  lrs[1] = __get_lr();
  sps[1] = __get_sp();

  change_mode(abort_mode);
  spsrs[1] = __get_spsr();
  lrs[2] = __get_lr();
  sps[2] = __get_sp();

  change_mode(fiq_mode);
  spsrs[2] = __get_spsr();
  lrs[3] = __get_lr();
  sps[3] = __get_sp();

  change_mode(irq_mode);
  spsrs[3] = __get_spsr();
  lrs[4] = __get_lr();
  sps[4] = __get_sp();

  change_mode(undefined_mode);
  spsrs[4] = __get_spsr();
  lrs[5] = __get_lr();
  sps[5] = __get_sp();

  change_mode(current_mode);
  /* MAKE SURE NONE OF THESE ARE PUT ON THE STACK!! */

  char buf[100];
  kprintf("\n>>> Current statusregisters (SPSR of current mode) <<<\n");

  kprintf("CPSR: %s\n", format_status_register(buf, cpsr));
  kprintf("SPSR: %s\n", format_status_register(buf, spsr));

  kprintf("\n>>> Current modespecific registers (except R8-R12) <<<\n");
  kprintf("              LR          SP          SPSR\n");

  kprintf("User/System:  %x  %x\n", lrs[0], sps[0]);

  kprintf("Supervisor:   %x  %x  %s\n", lrs[1], sps[1],
          format_status_register(buf, spsrs[0]), spsrs[0]);

  kprintf("Abort:        %x  %x  %s\n", lrs[2], sps[2],
          format_status_register(buf, spsrs[1]), spsrs[1]);

  kprintf("FIQ:          %x  %x  %s\n", lrs[3], sps[3],
          format_status_register(buf, spsrs[2]), spsrs[2]);

  kprintf("IRQ:          %x  %x  %s\n", lrs[4], sps[4],
          format_status_register(buf, spsrs[3]), spsrs[3]);

  kprintf("Undefined:    %x  %x  %s\n", lrs[5], sps[5],
          format_status_register(buf, spsrs[4]), spsrs[4]);
}

static inline void halt_system() {
  kprintf("\n\nSYSTEM HALT.");
  for (;;)
    ;
}

#endif
