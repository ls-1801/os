#include <stdint.h>

#define SVC_KILL_THREAD 0
#define SVC_YIELD_THREAD 1

#define SVC_PUTS 10
#define SVC_GETS 11

static inline void syscall(uint16_t syscallNo) {
  asm volatile("SVC %0" ::"I"(syscallNo));
}
