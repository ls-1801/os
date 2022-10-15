#ifndef _LIB_BITS_H_
#define _LIB_BITS_H_
#include <common.h>
#define UNSET_BIT(var, pos) (var &= ~(1 << (pos)))
#define SET_BIT(var, pos) (var |= (1 << (pos)))
#define CHECK_BIT(var, pos) (var & (1 << (pos)))
#define FFS(var) __builtin_clz(var)
#define ALIGN(addr, alignemnt) (addr - (addr % alignemnt))

static inline uint32_t __clz(uint32_t val) {
  int32_t ret;
  asm volatile("CLZ %0, %1" : "=r"(ret) : "r"(val));
  return ret;
}
#endif /* _LIB_BITS_H_ */
