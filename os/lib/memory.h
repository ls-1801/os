#ifndef _LIB_MEMORY_H_
#define _LIB_MEMORY_H_
#ifndef __cplusplus

#include <common.h>

size_t memcpy(void *dst, void *src, size_t length);
void *memset(void *s, uint8_t c, size_t n);

#endif /* __cplusplus */
#endif /* _LIB_MEMORY_H_ */