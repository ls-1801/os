#include "static_heap.h"
#include <gtest/gtest.h>

MK_HEAP(test_heap, 8, uint32_t);

void recursiveAlloc() {
  uint32_t *t = (uint32_t *)static_heap_allocate(test_heap);
  if (t) {
    recursiveAlloc();
    static_heap_free(test_heap, t);
    EXPECT_EQ(t, static_heap_allocate(test_heap));
  }
}

TEST(TestStaticHeap, TestAllocation) {
  recursiveAlloc();
  EXPECT_EQ(NULL, static_heap_allocate(test_heap));

  for (int i = test_heap->heap_size - 1; i >= 1; i--) {
    static_heap_free(test_heap, test_heap->static_heap_data + i * 4);
  }

  EXPECT_EQ(test_heap->static_heap_data + 4, static_heap_allocate(test_heap));

  recursiveAlloc();
  EXPECT_EQ(NULL, static_heap_allocate(test_heap));

  for (int i = test_heap->heap_size - 1; i >= 0; i--) {
    static_heap_free(test_heap, test_heap->static_heap_data + 4 * i);
  }

  EXPECT_EQ(test_heap->static_heap_data, static_heap_allocate(test_heap));
  heap_t heap = test_heap;
}
