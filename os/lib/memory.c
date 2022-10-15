#ifndef __cplusplus
#include "memory.h"

size_t memcpy(void *dst, void *src, size_t length) {
  size_t i;

  for (i = 0; i < length; i++) {
    ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
  }

  return i;
}

void *memset(void *s, uint8_t c, size_t n) {
  uint8_t *p = s;
  while (n--) {
    *p++ = c;
  }
  return s;
}

#endif /* __cplusplus */