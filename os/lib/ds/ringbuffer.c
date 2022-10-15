#include "ringbuffer.h"

size_t buffer_put(struct Buffer *buf, char c) {
  if (buf->len == buf->size)
    return 0;

  *buf->w++ = c;
  if (buf->w == buf->start + buf->size)
    buf->w = buf->start;

  buf->len++;
  return 1;
}

bool buffer_empty(struct Buffer *buf) { return buf->len == 0; }

size_t buffer_get(struct Buffer *buf, char *out, size_t len) {
  size_t i;
  char *buf_end = buf->start + buf->size;

  for (i = 0; i < len && buf->len > 0; i++) {
    *out++ = *buf->r++;
    buf->len--;
    if (buf->r == buf_end)
      buf->r = buf->start;
  }

  return i;
}

char buffer_first(struct Buffer *buf) { return *buf->r; }
