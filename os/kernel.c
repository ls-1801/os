#include "lib/io.h"
#include "lib/strings.h"
#include "lib/timer.h"
#include "limits.h"
#include "system/interrupts.h"
#include "system/mmu.h"
#include "system/register.h"
#include "system/scheduler.h"
#include "system/system.h"
#include "test/test_runner.h"

extern uint32_t __sys_stack_start;
extern uint32_t __und_stack_start;
extern uint32_t __fiq_stack_start;
extern uint32_t __irq_stack_start;
extern uint32_t __abt_stack_start;
extern uint32_t __svc_stack_start;
extern uint32_t __user_stack_start;

void start_kernel() {
  init_mmu();
  kprintf("MMU initialized!");
  kprintf("\n\n");
  run_all_tests();
  kprintf("Current Mode: %s\n", get_mode_description(get_mode()));
  kprintf("SP: %x\n", __get_sp());

  int __sys_stack_start_reg;
  asm volatile("ldr %0, =__sys_stack_start" : "=r"(__sys_stack_start_reg));

  kprintf("Stacks:\n"
          "sys: %x \t sys: %x\n"
          "irq: %x \t abt: %x\n"
          "fiq: %x \t svc: %x\n"
          "user: %x\n",
          __sys_stack_start_reg, &__sys_stack_start, &__irq_stack_start,
          &__abt_stack_start, &__fiq_stack_start, &__svc_stack_start,
          &__user_stack_start);

  kprintf("\n\nDemo ready!\nPress '1' to start a new Thread.\n");

  start_scheduling();
  enable_interrupts();

  for (;;)
    asm volatile("WFI");
}
