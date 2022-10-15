#ifndef _COMMON_H_
#define _COMMON_H_
#define NOP() asm volatile("nop")
#define NULL ((void *)0)
#define UNUSED(x) (void)(x)

#define TOKENPASTE(x, y) x##y
#define CONCAT_TOKEN(x, y) TOKENPASTE(x, y)
#define UNIQUE(varname) CONCAT_TOKEN(varname, __COUNTER__)

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
typedef enum { false = 0, true = 1 } bool;
#endif /* __cplusplus */

#endif /* _COMMON_H_ */