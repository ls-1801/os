#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "static_heap.h"
#include "../bits.h"
#include "../memory.h"

void *static_heap_allocate(heap_t heap) {
  uint32_t allocation_index;
  for (allocation_index = 0; allocation_index < heap->heap_size / 32u;
       allocation_index++) {
    if (~(heap->allocation[allocation_index]) == 0)
      continue;

    uint32_t pos = FFS(~(heap->allocation[allocation_index]));
    if (pos < 32) {
      SET_BIT(heap->allocation[allocation_index], 31 - pos);
      return heap->static_heap_data +
             (heap->type_size * (allocation_index * 32 + pos));
    }
  }

  uint8_t pos = FFS(~(heap->allocation[((heap->heap_size - 1) / 32u)]));
  if (pos < (heap->heap_size % 32u)) {
    SET_BIT(heap->allocation[((heap->heap_size - 1) / 32u)], 31u - pos);
    return heap->static_heap_data +
           (heap->type_size * (((heap->heap_size - 1) / 32u) * 32u + pos));
  }

  return NULL;
}

void static_heap_free(heap_t heap, void *t) {
  uint32_t pos = ((char *)t - (char *)heap->static_heap_data) / heap->type_size;
  if (pos >= heap->heap_size)
    return;

  UNSET_BIT(heap->allocation[pos / 32u], 31 - (pos % 32u));
}

uint32_t static_heap_position(heap_t heap, void *t) {
  return ((char *)t - (char *)heap->static_heap_data) / heap->type_size;
}
#ifdef __cplusplus
}
#endif /* __cplusplus */