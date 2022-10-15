#ifndef _LIB_DS_RINGBUFFER_H_
#define _LIB_DS_RINGBUFFER_H_

#include <common.h>

struct Buffer {
  char *r;
  char *w;
  char *start;
  size_t len;
  size_t size;
};

typedef struct Buffer *rbuf_t;

size_t buffer_put(rbuf_t, char c);
bool buffer_empty(rbuf_t);
size_t buffer_get(rbuf_t, char *out, size_t len);
char buffer_first(rbuf_t);

#endif /* _LIB_DS_RINGBUFFER_H_ */