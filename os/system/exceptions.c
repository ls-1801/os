#include "../lib/bits.h"
#include "../lib/io.h"
#include "debugutil.h"
#include "mmu.h"
#include "register.h"
#include "scheduler.h"
#include "system.h"
#include <common.h>

#define WNR 11

static const char *error_text[] = {
    NULL,
    "Alignment fault",
    "Debug Event",
    "Access flag fault - First Level",
    "Fault on instruction cache maintenance",
    "Translation fault - First Level",
    "Access flag fault - Second Level",
    "Translation fault - Second Level",
    "Synchronous external abort",
    "Domain fault - First Level",
    NULL,
    "Domain fault - Second Level",
    "Synchronous external abort on translation table walk - First Level",
    "Permission fault - First Level",
    "Synchronous external abort on translation table walk - Second Level",
    "Permission fault - Second Level",
    "TLB conflict abort",
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "Asynchronous external abort",
    NULL,
    "Asynchronous parity error on memory access",
    "Synchronous parity error on memory access",
    NULL,
    NULL,
    "Synchronous parity error on translation table walk- First Level",
    NULL,
    "Synchronous parity error on translation table walk- Second Level",
};

static const char *error_text_ifsr[] = {
    "No function, reset value",
    "Alignment fault",
    "Debug event fault",
    "Access Flag fault on Section",
    "No function[2]",
    "Translation fault on Section",
    "Access Flag fault on Page",
    "Translation fault on Page",
    "Precise External Abort",
    "Domain fault on Section",
    "No function",
    "Domain fault on Page",
    "External abort on translation, first level",
    "Permission fault on Section",
    "External abort on translation, second level",
    "Permission fault on Page",
};

static uint32_t __get_DFAR() {
  int ret;
  asm("MRC p15, 0, %0, c6, c0, 0" : "=r"(ret));
  return ret;
}

static uint32_t __get_DFSR() {
  int ret;
  asm("MRC p15, 0, %0, c5, c0, 0" : "=r"(ret));
  return ret;
}

static uint32_t __get_IFSR() {
  int ret;
  asm("MRC p15, 0, %0, c5, c0, 1" : "=r"(ret));
  return ret;
}

static uint32_t __get_IFAR() {
  int ret;
  asm("MRC p15, 0, %0, c6, c0, 2" : "=r"(ret));
  return ret;
}

static uint32_t get_abort_addr() { return __get_DFAR(); }

static const char *get_abort_error() {
  int dfsr = __get_DFSR();
  int fs = (dfsr & (1 << 10) >> 6) + (dfsr & 0xf);
  return error_text[fs];
}

static const char *get_abort_error_IFSR() {
  int dfsr = __get_IFSR();
  int fs = dfsr & 0xF;
  return error_text_ifsr[fs];
}

static const char *get_access_type() {
  if (CHECK_BIT(__get_DFSR(), WNR))
    return "writing";
  else
    return "reading";
}

static uint32_t get_undefined_instruction(uint32_t lr) {
  uint32_t ret;
  asm volatile("LDR %0, [%1, #-4]" : "=r"(ret) : "r"(lr));
  return ret;
}

static uint32_t *get_undefined_instruction_adress(uint32_t lr) {
  /**
   * An undefined instruction is detected during the Instruction Decode (ID)
   * phase hence the program counter was one instruction ahead when the
   * exception occured.
   * */
  return (uint32_t *)lr - 1;
}

static uint32_t get_abort_instruction(uint32_t lr) {
  uint32_t ret;
  asm volatile("LDR %0, [%1, #-8]" : "=r"(ret) : "r"(lr));
  return ret;
}

void undefined_instruction(uint32_t *regs) {
  uint32_t cpsr = __get_cpsr();
  uint32_t spsr = __get_spsr();

  print_bar();
  kprintf("Undefined Instruction!\nCaused by %x at %x\n",
          get_undefined_instruction(regs[PC]),
          get_undefined_instruction_adress(regs[PC]));

  register_snapshot(regs);
  status_register_snapshot(cpsr, spsr, undefined_mode);

  if (is_user_mode(spsr)) {
    kill_active_thread();
    return;
  }

  kprintf("SYSTEM HALT!");
  for (;;)
    ;
}

void prefetch_abort(uint32_t *regs) {
  uint32_t cpsr = __get_cpsr();
  uint32_t spsr = __get_spsr();
  uint32_t ifar = __get_IFAR();

  print_bar();
  kprintf("Prefetch Abort!\n");
  kprintf("At address %x (%s)\n", ifar, get_section(ifar));
  kprintf("Error: %s\n\n", get_abort_error_IFSR());

  register_snapshot(regs);
  status_register_snapshot(cpsr, spsr, abort_mode);

  /* kill user thread */
  if (is_user_mode(spsr)) {
    kill_active_thread();
    return;
  }

  kprintf("SYSTEM HALT!");
  for (;;)
    ;
}

void data_abort(uint32_t *regs, int lr) {
  uint32_t cpsr = __get_cpsr();
  uint32_t spsr = __get_spsr();
  uint32_t abort_address = get_abort_addr();
  print_bar();
  kprintf("Data Abort!\nCaused by %x at %p \n", get_abort_instruction(lr), lr);
  kprintf("Access: %s at address %p (%s)\n", get_access_type(), abort_address,
          get_section(abort_address));
  kprintf("Error: %s\n\n", get_abort_error());

  register_snapshot(regs);
  status_register_snapshot(cpsr, spsr, abort_mode);

  /* kill user thread */
  if (is_user_mode(spsr)) {
    kill_active_thread();
    return;
  }

  kprintf("SYSTEM HALT!");
  for (;;)
    ;
}