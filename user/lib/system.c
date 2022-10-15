#include "system.h"
#include <svc.h>

void __exit(uint8_t sig) {
  asm volatile("MOV r0, %0" ::"r"(sig));
  syscall(SVC_KILL_THREAD);
}