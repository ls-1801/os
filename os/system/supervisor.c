
#include "debugutil.h"
#include "scheduler.h"
#include "svc.h"
#include "thread.h"

extern uint32_t *__user_stack_end;
extern uint32_t *__user_stack_start;

static bool legal_address(void *ptr) {
  // check if address is legal, which currently means it is inside the (current
  // thread) user stack.
  return ptr <= (void *)&__user_stack_start && ptr >= (void *)&__user_stack_end;
}

static bool legal_ro_address(void *ptr) {
  // Either User Stack or User R0 Data.
  return legal_address(ptr) ||
         (ptr >= (void *)0x800000 && ptr <= (void *)0x900000);
}

uint32_t get_svc_no(uint32_t pc) { return *((uint32_t *)pc) & 0x00FFFFFF; }

static void puts_svc() {
  thread_t t = get_active_thread();
  if (!t) {
    return;
  }

  save_thread(t);
  char *str = (char *)get_thread_register(t, r0);

  if (!legal_ro_address(str)) {
    kprintf("[System] Thread uses an illegal address (%x) in PUTS_SVC", str);
    return;
  }

  kprintf("%s", str);
  load_thread(t);
}

static void gets_svc() {
  thread_t t = get_active_thread();
  if (!t) {
    return;
  }

  save_thread(t);
  char *buf = (char *)get_thread_register(t, r0);

  if (!legal_address(buf)) {
    kprintf("[System] Thread uses an illegal address (%x) in GETS_SVC", buf);
    return;
  }

  uint32_t length = get_thread_register(t, r1);
  uint32_t read_bytes = read(buf, length);
  set_thread_register(t, r0, read_bytes);
  load_thread(t);
}

void software_interrupt(uint32_t *regs, uint32_t lr) {
  UNUSED(regs);

  uint32_t spsr = __get_spsr();
  uint32_t svc_no = get_svc_no(lr);

  switch (svc_no) {
  case SVC_KILL_THREAD:
    kill_active_thread();
    break;
  case SVC_YIELD_THREAD:
    yield_current_thread();
    break;
  case SVC_PUTS:
    puts_svc();
    break;
  case SVC_GETS:
    gets_svc();
    break;
  default:
    kprintf("UNKNOWN SYSCALL. ");
    if (is_user_mode(spsr)) {
      kprintf("Bye bye thread.\n");
      kill_active_thread();
    } else {
      halt_system();
    }
  }
}