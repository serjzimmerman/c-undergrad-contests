#ifdef D_DEBUG
#define NDEBUG
#endif

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdlib.h>

#include "sllistc.h"

struct sl_node_t {
  struct sl_node_t *next;
  void *data;
};

struct sl_node_t *sl_node_init() {
  struct sl_node_t *node;

  node = calloc(1, sizeof(struct sl_node_t));

  if (!node) {
    return NULL;
  }

  return node;
}

void *sl_node_get_data(struct sl_node_t *const node) {
  assert(node);

  return node->data;
}

void sl_node_free(struct sl_node_t *const node) {
  assert(node);

  free(node);
}

void sl_node_set_data(struct sl_node_t *const node, void *const data) {
  assert(node);

  node->data = data;
}

struct sl_node_t *sl_node_get_n_next(struct sl_node_t *const node, int n) {
  struct sl_node_t *curr;
  
  curr = node;

  for (; n > 0; n--) {
    if (!curr) {
      return NULL;
    }
    curr = curr->next;
  }

  return curr;
}

struct sl_list_t {
  struct sl_node_t *head, *tail;
};

struct sl_list_t *sl_list_init() {
  struct sl_list_t *list;

  list = calloc(1, sizeof(struct sl_list_t));

  if (!list) {
    return NULL;
  }

  return list;
}

void sl_list_free(struct sl_list_t *const list, void (*datafree)(void *data)) {
  struct sl_node_t *curr, *next;

  curr = list->head;

  if (datafree) {
    while (curr) {
      next = curr->next;
      datafree(sl_node_get_data(curr));
      sl_node_free(curr);
      curr = next;
    }
  } else {
    while (curr) {
      next = curr->next;
      sl_node_free(curr);
      curr = next;
    }
  }

  free(list);
}

int sl_list_is_empty(const struct sl_list_t *const list) {
  assert(list);

  if (list->head) {
    return 0;
  }

  return 1;
}

struct sl_node_t *sl_list_get_head(struct sl_list_t *const list) {
  assert(list);

  return list->head;
}

struct sl_list_t *sl_list_push(struct sl_list_t *const list, struct sl_node_t *const node) {
  assert(list);
  assert(node);

  if (sl_list_is_empty(list)) {
    list->head = node;
    list->tail = node;
    node->next = NULL;
  } else {
    node->next = list->head;
    list->head = node;
  }

  return list;
}

struct sl_list_t *sl_list_append(struct sl_list_t *const list, struct sl_node_t *const node) {
  assert(list);
  assert(node);

  if (sl_list_is_empty(list)) {
    list->head = node;
    list->tail = node;
    node->next = NULL;
  } else {
    list->tail->next = node;
    list->tail = node;
  }

  return list;
}

void sl_list_insert_after(struct sl_list_t *const list, struct sl_node_t *const prev, struct sl_node_t *node) {
  assert(list);
  assert(node);

  if (list->tail == prev) {
    prev->next = node;
    node->next = NULL;
    list->tail = node;
  } else {
    node->next = prev->next;
    prev->next = node;
  }
}

/* TODO[]: Redo, kinda jank when used in callback function to sl_list_iterate_over_nodes() */
void sl_list_remove_node(struct sl_list_t *const list, struct sl_node_t *prev, struct sl_node_t *node) {
  assert(list);
  assert(node);
  
  if (list->head == list->tail) {
    list->head = NULL;
    list->tail = NULL;
  } else if (list->head == node) {
    list->head = node->next;
    node->next = NULL;
  } else {
    prev->next = node->next;
    node->next = NULL;
  }
}

void sl_list_iterate_over_nodes(struct sl_list_t *const list,
                                void (*callback)(struct sl_node_t *node, struct sl_list_t *list,
                                                 va_list argp),
                                ...) {
  struct sl_node_t *curr, *prev, *next;
  va_list argp, argcopy;

  assert(list);

  va_start(argp, callback);
  curr = list->head;
  prev = NULL;

  while (curr) {
    va_copy(argcopy, argp);
    next = curr->next;
    callback(curr, list, argcopy);
    prev = curr;
    curr = next;
    va_end(argcopy);
  }

  va_end(argp);
}

struct sl_node_t *_sl_node_get_middle(struct sl_node_t *head_ref) {
  struct sl_node_t *fast, *slow;

  assert(head_ref);

  fast = head_ref->next;
  slow = head_ref;

  while (fast) {
    fast = fast->next;
    if (fast) {
      slow = slow->next;
      fast = fast->next;
    }
  }

  return slow;
}

struct sl_node_t *_sl_list_merge_sorted(struct sl_node_t *a, struct sl_node_t *b, int (*cmp_func)(void *a, void *b)) {
  struct sl_node_t new_node, *tail_node, *next_node;

  tail_node = &new_node;

  while (a && b) {
    next_node = NULL;
    if (cmp_func(a->data, b->data) > 0) {
      next_node = b;
      b = b->next;
    } else {
      next_node = a;
      a = a->next;
    }
    tail_node->next = next_node;

    tail_node = tail_node->next;
  }

  if (a) {
    tail_node->next = a;
  } else {
    tail_node->next = b;
  }

  tail_node = new_node.next;

  return tail_node;
}

struct sl_node_t *_sl_node_merge_sort(struct sl_node_t *head, int (*cmp_func)(void *a, void *b)) {
  struct sl_node_t *middle_node, *left_node, *right_node;

  if (!head || !(head->next)) {
    return head;
  }

  middle_node = _sl_node_get_middle(head);
  left_node = head;
  right_node = middle_node->next;
  middle_node->next = NULL;

  left_node = _sl_node_merge_sort(left_node, cmp_func);
  right_node = _sl_node_merge_sort(right_node, cmp_func);

  return _sl_list_merge_sorted(left_node, right_node, cmp_func);
}

void sl_list_merge_sort(struct sl_list_t *list, int (*cmp_func)(void *a, void *b)) {
  struct sl_node_t *curr;

  assert(list);
  assert(cmp_func);

  list->head = _sl_node_merge_sort(list->head, cmp_func);

  /* Recalculate list->last node */

  curr = list->head;
  while (curr->next) {
    curr = curr->next;
  }

  list->tail = curr;
}