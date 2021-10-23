#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
  void *data;
  struct Node *next;
} * node;

typedef struct LinkedList {
  node first, last;
} * linkedlist;

typedef struct Data {
  void *data;
  int size;
} * data_t;

typedef int (*xcmp_t)(void *lhs, int lsz, void *rhs, int rsz);

node node_init() {
  node newnode;

  if (!(newnode = (node)calloc(1, sizeof(struct Node)))) {
    return NULL;
  }

  return newnode;
}

linkedlist linkedlist_init() {
  linkedlist newlist;

  if (!(newlist = (linkedlist)calloc(1, sizeof(struct LinkedList)))) {
    return NULL;
  }

  return newlist;
}

int _linkedlist_pre_append_push(linkedlist list, node *newnode, void *ptr) {
  if (!list) {
    return -1;
  }
  if (!ptr) {
    return -1;
  }
  if (!(*newnode = node_init())) {
    return -1;
  }

  (*newnode)->data = ptr;

  if (!list->first) {
    if (list->last) {
      return -1;
    }
    list->first = *newnode;
    list->last = *newnode;
    return 0;
  }

  return 0;
}

int linkedlist_append(linkedlist list, void *ptr) {
  node newnode = NULL;

  if (_linkedlist_pre_append_push(list, &newnode, ptr) != 0) {
    return -1;
  }

  if (newnode != list->first) {
    newnode->next = NULL;
    list->last->next = newnode;
    list->last = newnode;
  }

  return 0;
}

void _node_free_recursive(node current_node) {
  node temp;

  if (!(temp = current_node)) {
    return;
  }

  current_node = temp->next;

  free(temp);

  _node_free_recursive(current_node);
}

int linkedlist_free(linkedlist list) {
  if (!list) {
    return -1;
  }
  if (!list->first) {
    if (list->last) {
      return -1;
    }
    return 0;
  }

  _node_free_recursive(list->first);

  free(list);

  return 0;
}

node _node_get_middle(node head_ref) {
  node fast, slow;

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

node _merge_sorted_lists(node a, node b,
                         int (*cmp_func)(void *a, void *b, xcmp_t cmp),
                         xcmp_t cmp) {
  node new_node, next_node, tail_node;
  new_node = node_init();
  tail_node = new_node;

  while (a && b) {
    next_node = NULL;
    if (cmp_func(a->data, b->data, cmp) > 0) {
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

  tail_node = new_node->next;

  free(new_node);
  return tail_node;
}

node _node_merge_sort(node head_ref,
                      int (*cmp_func)(void *a, void *b, xcmp_t cmp),
                      xcmp_t cmp) {
  node middle_node, left_node, right_node;

  if (!head_ref || !(head_ref->next)) {
    return head_ref;
  }

  middle_node = _node_get_middle(head_ref);
  left_node = head_ref, right_node = middle_node->next;
  middle_node->next = NULL;

  left_node = _node_merge_sort(left_node, cmp_func, cmp);
  right_node = _node_merge_sort(right_node, cmp_func, cmp);

  return _merge_sorted_lists(left_node, right_node, cmp_func, cmp);
}

void linkedlist_merge_sort(linkedlist list,
                           int (*cmp_func)(void *a, void *b, xcmp_t cmp),
                           xcmp_t cmp) {
  list->first = _node_merge_sort(list->first, cmp_func, cmp);
}

void linkedlist_iterate_over_nodes(linkedlist list,
                                   void (*callback)(node node_to_iterate)) {
  node current_node;

  current_node = list->first;

  while (current_node) {
    callback(current_node);
    current_node = current_node->next;
  }
}

data_t init_data(void *data, int size) {
  data_t r = malloc(sizeof(struct Data));

  r->data = data;
  r->size = size;

  return r;
}

void free_data_t(data_t data) {
  free(data);
}

linkedlist list_from_mem(void *mem, int *sizes, int nelts) {
  data_t t;
  linkedlist list;
  char *curr;
  int i;

  list = linkedlist_init();
  curr = (char *)mem;

  for (i = 0; i < nelts; i++) {
    t = init_data(curr, sizes[i]);

    curr += sizes[i];

    if (linkedlist_append(list, t)) {
      abort();
    }
  }

  return list;
}

void list_to_mem(linkedlist list, void *mem, int *sizes) {
  node current_node = NULL;
  char *curr_mem = NULL, *curr_sizes = NULL;

  curr_mem = (char *)mem;
  curr_sizes = (char *)sizes;

  current_node = list->first;

  while (current_node) {
    memcpy(curr_mem, ((data_t)current_node->data)->data,
           ((data_t)current_node->data)->size);
    memcpy(curr_sizes, &((data_t)current_node->data)->size, sizeof(int));

    curr_mem += ((data_t)current_node->data)->size;
    curr_sizes += sizeof(int);

    current_node = current_node->next;
  }
}

int my_cmp(void *lhs, int lsz, void *rhs, int rsz) {
  return *(int *)lhs - *(int *)rhs;
}

int data_cmp(void *l, void *r, xcmp_t cmp) {
  data_t left_data, right_data;

  left_data = (data_t)l;
  right_data = (data_t)r;

  return cmp(left_data->data, left_data->size, right_data->data,
             right_data->size);
}

void free_data_all(linkedlist list) {
  node current_node;

  current_node = list->first;

  while (current_node) {
    free_data_t((data_t)current_node->data);
    current_node = current_node->next;
  }
}

int get_sum_of_array(int *array, int nelts) {
  int i, r;

  r = 0;

  for (i = 0; i < nelts; i++) {
    r += array[i];
  }

  return r;
}

void xmsort(void *mem, int *sizes, int nelts, xcmp_t cmp) {
  linkedlist list;
  void *new_mem;
  int size;

  list = list_from_mem(mem, sizes, nelts);
  linkedlist_merge_sort(list, data_cmp, cmp);

  size = get_sum_of_array(sizes, nelts);
  new_mem = malloc(size);

  list_to_mem(list, new_mem, sizes);

  memcpy(mem, new_mem, size);

  free(new_mem);
  free_data_all(list);
  linkedlist_free(list);
}

int main() {
  xcmp_t cmp;

  int array[] = {3, 10, 5, 20, 40, 100, 59, 123};
  int sizes[] = {4, 4, 4, 4, 4, 4, 4, 4};

  cmp = my_cmp;

  xmsort(&array[0], &sizes[0], 8, cmp);

  return 0;
}