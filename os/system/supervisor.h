#ifndef _SYSTEM_SUPERVISOR_H_
#define _SYSTEM_SUPERVISOR_H_

#include <common.h>
#include <svc.h>

static inline void syscall(uint16_t syscallNo) {
  asm volatile("SVC %0" ::"I"(syscallNo));
}

#endif /* _SYSTEM_SUPERVISOR_H_ */