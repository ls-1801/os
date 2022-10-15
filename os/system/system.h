#ifndef _SYSTEM_H
#define _SYSTEM_H

#include "register.h"
#include <common.h>

#define CPSR_IRQ BIT(7)
#define CPSR_FIQ BIT(6)

#define change_irq_free_system_mode() (__set_cpsr(0b11011111))

static inline void sleep(uint32_t cycles) {
  uint32_t i = 0;
  for (; i < cycles; i++) {
    asm("nop");
  }
}

enum arm_mode {
  user_mode = 0b10000,
  fiq_mode = 0b10001,
  irq_mode = 0b10010,
  svc_mode = 0b10011,
  abort_mode = 0b10111,
  undefined_mode = 0b11011,
  system_mode = 0b11111,
};

static inline void change_mode(enum arm_mode mode) {
  switch (mode) {
  case system_mode:
    asm("CPS %0" ::"I"(system_mode));
    break;
  case irq_mode:
    asm("CPS %0" ::"I"(irq_mode));
    break;
  case fiq_mode:
    asm("CPS %0" ::"I"(fiq_mode));
    break;
  case svc_mode:
    asm("CPS %0" ::"I"(svc_mode));
    break;
  case user_mode:
    asm("CPS %0" ::"I"(user_mode));
    break;
  case abort_mode:
    asm("CPS %0" ::"I"(abort_mode));
    break;
  case undefined_mode:
    asm("CPS %0" ::"I"(undefined_mode));
    break;
  }
}

static inline enum arm_mode get_mode() { return __get_cpsr() & 0b11111; }
static inline bool is_user_mode(uint32_t spsr) {
  return (spsr & 0b11111) == user_mode;
}

static inline void enable_interrupts() { asm("CPSIE i"); }

static inline void disable_interrupts() { asm("CPSID i"); }

static inline const char *get_mode_description(int32_t status_register) {
  switch (status_register & 0b11111) {
  case system_mode:
    return "System";
  case irq_mode:
    return "IRQ";
  case fiq_mode:
    return "FIQ";
  case svc_mode:
    return "Supervisor";
  case user_mode:
    return "User";
  case abort_mode:
    return "Abort";
  case undefined_mode:
    return "Undefined";
  default:
    return "Literally Undefined";
  }
}

#endif
