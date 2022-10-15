#include "thread.h"
#include "../lib/bits.h"
#include "../lib/ds/static_heap.h"
#include "../lib/io.h"
#include "../lib/memory.h"
#include "system.h"
#include <stdarg.h>
#include <svc.h>

#define THREAD_STACK_OFFSET 1024 * 10
#define THREAD_STACK_SIZE 1024 * 10
#define USR_MODE 0b10000
#define ENDIANNESS (1 << 9)

// #define USER_STACK_START 0x700000
// #define USER_STACK_END 0x600000

void *__current_thread_registers;
uint32_t __current_thread_spsr;

// Linker Script
extern uint32_t *__user_stack_start;
extern uint32_t *__user_stack_end;

struct Thread {
  uint32_t id;
  uint32_t regs[14]; // r0 - r12 + pc
  uint32_t sp;
  uint32_t lr;
  uint32_t cpsr;
};

MK_HEAP(thread_heap, MAX_THREADS, struct Thread);
static heap_t threads = &thread_heap;

thread_t allocate_thread() {
  thread_t t = (thread_t)static_heap_allocate(threads);
  t->id = static_heap_position(threads, t);
  return t;
}
void free_thread(thread_t t) { static_heap_free(threads, t); }

static uint8_t thread_id(thread_t t) { return t->id; }
void *get_thread_sp(thread_t t) { return (void *)t->sp; }
void set_thread_sp(thread_t t, void *ptr) { t->sp = (uint32_t)ptr; }
void set_thread_argument(thread_t t, uint32_t arg) { t->regs[0] = arg; }
void set_thread_pc(thread_t t, void *pc) { t->regs[13] = (uint32_t)pc; }

char *debug_thread(thread_t t, char *buf) {
  return sprintf(buf,
                 "Thread Id: %d, CPSR: %x\nRegisters:\n"
                 "R0:\t%x\tR8:\t%x\n"
                 "R1:\t%x\tR9:\t%x\n"
                 "R2:\t%x\tR10:\t%x\n"
                 "R3:\t%x\tR11:\t%x\n"
                 "R4:\t%x\tR12:\t%x\n"
                 "R5:\t%x\tSP:\t%x\n"
                 "R6:\t%x\tLR:\t%x\n"
                 "R7:\t%x\tPC:\t%x\n",
                 thread_id(t), t->cpsr, t->regs[0], t->regs[8], t->regs[1],
                 t->regs[9], t->regs[2], t->regs[10], t->regs[3], t->regs[11],
                 t->regs[4], t->regs[12], t->regs[5], t->sp, t->regs[6], t->lr,
                 t->regs[7], t->regs[13]);
}

void thread_killer() { syscall(SVC_KILL_THREAD); }

void initialize_thread(thread_t t) {
  uint32_t idx = thread_id(t);

  // Init Registers.
  memset(t->regs, 0xCC, sizeof t->regs);

  // First Threads Stack start is __sys_stack - THREAD_STACK_OFFSET
  uint32_t thread_stack =
      (uint32_t)&__user_stack_start - (idx * THREAD_STACK_SIZE);

  kprintf("New Thread Stack starts at %x\n", thread_stack);

  if (thread_stack < (uint32_t)&__user_stack_end) {
    kprintf("USER STACK IS NO LONGER IN THE USER STACK SEGMENT!: %x",
            thread_stack);
  }

  // Initialize Stack. Align to 4 Byte.
  t->sp = ALIGN(thread_stack, 4);

  // Set LR to thread_killer, which makes the thread exit once it reaches the
  // end of the threads function.
  t->lr = (uint32_t)thread_killer;

  // Init CPSR
  t->cpsr = USR_MODE | ENDIANNESS;
}

/* Load Thread */
void load_thread(register thread_t t) {
  /* PUT R0-R12 and the PC (LR - 4) onto the handler Stack ready to be loaded
   * once the handler exists */
  memcpy(__current_thread_registers, t->regs, sizeof t->regs);

  /* Change to SYSTEM Mode to change SP and LR */
  /* Generated Assembly stores t in R5 no stack usage. */

  register enum arm_mode current_mode = get_mode();
  change_irq_free_system_mode();
  __set_sp(t->sp);
  __set_lr(t->lr);
  change_mode(current_mode);
  /* spsr will be copied into the cpsr once the registers are loaded from the
   * stack */
  __set_spsr(t->cpsr);
}

/* Store Thread */
void save_thread(register thread_t t) {
  /* Copy current Registers from Handler Stack */
  memcpy(t->regs, __current_thread_registers, sizeof t->regs);

  /* Previous threads cpsr is in the handlers spsr unless something happened in
   * between */
  t->cpsr = __current_thread_spsr;
  /* Change to System Mode to read the User/Sys Mode specifc Registers. */
  register enum arm_mode current_mode = get_mode();
  change_irq_free_system_mode();
  t->sp = __get_sp();
  t->lr = __get_lr();
  change_mode(current_mode);
}

uint32_t get_thread_register(thread_t t, enum ThreadRegister tr) {
  return t->regs[tr];
}

void set_thread_register(thread_t t, enum ThreadRegister tr, uint32_t v) {
  t->regs[tr] = v;
}