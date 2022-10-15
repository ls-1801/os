#ifndef _LIB_DS_STATIC_HEAP_H_
#define _LIB_DS_STATIC_HEAP_H_
#include <common.h>

typedef const struct static_heap *heap_t;
struct static_heap {
  uint32_t heap_size;
  uint32_t type_size;
  void *static_heap_data;
  uint32_t *allocation;
};

#define __MK_HEAP_INTERNAL(HEAP_NAME, HEAP_SIZE, TYPE, HEAP_DATA,              \
                           HEAP_ALLOCATIONS, HEAP_STRUCT_NAME)                 \
  static TYPE HEAP_DATA[HEAP_SIZE];                                            \
  static uint32_t HEAP_ALLOCATIONS[((HEAP_SIZE - 1) / 32) + 1] = {0};          \
  static const struct static_heap HEAP_NAME = {HEAP_SIZE, sizeof(TYPE),        \
                                               HEAP_DATA, HEAP_ALLOCATIONS};

#define MK_HEAP(HEAP_NAME, HEAP_SIZE, TYPE)                                    \
  __MK_HEAP_INTERNAL(HEAP_NAME, HEAP_SIZE, TYPE, UNIQUE(__heap_data),          \
                     UNIQUE(__heap_allocations), UNIQUE(__static_heap))

void *static_heap_allocate(heap_t heap);
uint32_t static_heap_position(heap_t, void *t);
void static_heap_free(heap_t heap, void *t);

#endif /* _LIB_DS_STATIC_HEAP_H_ */