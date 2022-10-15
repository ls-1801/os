#ifndef _LIB_STATIC_LINKED_LIST_H_
#define _LIB_STATIC_LINKED_LIST_H_
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "../bits.h"
#include "../memory.h"
#include "static_heap.h"
#include <common.h>

typedef struct sll *sll_t;
typedef struct sll_element *sll_list_element_t;
struct sll {
  uint32_t list_size;
  uint32_t type_size;
  uint32_t list_length;
  heap_t node_heap;
  heap_t type_heap;
  sll_list_element_t first;
  sll_list_element_t last;
};

struct sll_element {
  sll_list_element_t next;
  void *data;
};

/**
 * MACRO WARNING.
 * Create Two Heaps. One for Data one for Internal Nodes.
 * Use both heaps to build the Static Linked List and makes sure unique names
 * are used
 */
#define MK_LIST_INTERNAL(NAME, TYPE, SIZE, NODE_HEAP_NAME, TYPE_HEAP_NAME,     \
                         STRUCT_NAME)                                          \
  static TYPE CONCAT_TOKEN(TYPE_HEAP_NAME, _data)[SIZE];                       \
  static uint32_t CONCAT_TOKEN(TYPE_HEAP_NAME,                                 \
                               _alloc)[((SIZE - 1) / 32) + 1] = {0};           \
  static struct static_heap TYPE_HEAP_NAME = {                                 \
      SIZE, sizeof(TYPE), CONCAT_TOKEN(TYPE_HEAP_NAME, _data),                 \
      CONCAT_TOKEN(TYPE_HEAP_NAME, _alloc)};                                   \
  static struct sll_element CONCAT_TOKEN(NODE_HEAP_NAME, _data)[SIZE];         \
  static uint32_t CONCAT_TOKEN(NODE_HEAP_NAME,                                 \
                               _alloc)[((SIZE - 1) / 32) + 1] = {0};           \
  static struct static_heap NODE_HEAP_NAME = {                                 \
      SIZE, sizeof(struct sll_element), CONCAT_TOKEN(NODE_HEAP_NAME, _data),   \
      CONCAT_TOKEN(NODE_HEAP_NAME, _alloc)};                                   \
  static struct sll STRUCT_NAME = {                                            \
      SIZE, sizeof(TYPE), 0, (heap_t)&NODE_HEAP_NAME, (heap_t)&TYPE_HEAP_NAME, \
      NULL, NULL};                                                             \
  sll_t NAME = &STRUCT_NAME;

#define MK_LIST_INTERNAL_1(NAME, TYPE, SIZE, NODE_HEAP_NAME, TYPE_HEAP_NAME,   \
                           STRUCT_NAME)                                        \
  MK_LIST_INTERNAL(UNIQUE(NODE_HEAP_NAME), UNIQUE(NODE_HEAP_NAME), SIZE,       \
                   UNIQUE(STRUCT_NAME));

#define MK_LIST(NAME, TYPE, SIZE)                                              \
  MK_LIST_INTERNAL(NAME, TYPE, SIZE, CONCAT_TOKEN(NAME, _node_heap),           \
                   CONCAT_TOKEN(NAME, _type_heap),                             \
                   CONCAT_TOKEN(NAME, _struct));

sll_list_element_t sll_allocate(sll_t list);

void sll_free(sll_t list, sll_list_element_t el);

void sll_insert_front(sll_t list, void *data);
void sll_insert_back(sll_t list, void *data);

void *sll_get_first(sll_t list);
void sll_rotate(sll_t list);

void sll_pop_first(sll_t list, void *out);

uint32_t sll_length(sll_t list);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _LIB_STATIC_LINKED_LIST_H_ */