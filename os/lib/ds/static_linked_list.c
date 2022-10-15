#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include "static_linked_list.h"

sll_list_element_t sll_allocate(sll_t list) {
  sll_list_element_t el =
      (sll_list_element_t)static_heap_allocate(list->node_heap);
  el->data = static_heap_allocate(list->type_heap);
  return el;
}

void sll_free(sll_t list, sll_list_element_t el) {
  static_heap_free(list->type_heap, el->data);
  static_heap_free(list->node_heap, el);
}

void sll_insert_front(sll_t list, void *data) {
  sll_list_element_t el = sll_allocate(list);
  memcpy(el->data, data, list->type_size);
  el->next = list->first; /* next is old first*/

  list->first = el; /* update first*/

  if (list->list_length == 0) /* special case empty list. also update last.*/
    list->last = el;

  list->list_length++;
}

void sll_insert_back(sll_t list, void *data) {
  sll_list_element_t el = sll_allocate(list);
  memcpy(el->data, data, list->type_size);
  el->next = NULL; /* at the end*/

  if (list->last) /* if a last element exist update its next ptr*/
    list->last->next = el;

  list->last = el; /* update the last element*/

  if (list->list_length == 0) /* special case empty list. also update first.*/
    list->first = el;

  list->list_length++;
}

void *sll_get_first(sll_t list) {
  if (!list->first)
    return NULL;

  return list->first->data;
}

void sll_rotate(sll_t list) {
  if (!list->first || list->first == list->last)
    return;

  sll_list_element_t first = list->first;
  sll_list_element_t last = list->last;

  sll_list_element_t new_first = first->next;
  sll_list_element_t new_last = list->first;

  first->next = NULL;
  last->next = list->first;

  list->first = new_first;
  list->last = new_last;
}

void sll_pop_first(sll_t list, void *out) {

  sll_list_element_t new_first = list->first->next;
  memcpy(out, list->first->data, list->type_size);
  sll_free(list, list->first);
  list->first = new_first;

  list->list_length--;
  if (list->list_length == 0) { /* special case invalidate last element.*/
    list->last = NULL;
  }
}

uint32_t sll_length(sll_t list) { return list->list_length; }
#ifdef __cplusplus
}
#endif /* __cplusplus */
