#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  void        *data;
  struct Node *next;
} * node;

typedef struct LinkedList {
  node first, last;
} * linkedlist;

typedef struct Data {
  int *array;
  int  len;
} * data_t;

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
    list->last  = *newnode;
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
    newnode->next    = NULL;
    list->last->next = newnode;
    list->last       = newnode;
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

node _merge_sorted_lists(node a, node b, int (*cmp_func)(void *a, void *b)) {
  node new_node, tail_node;

  new_node  = node_init();
  tail_node = new_node;

  while (a && b) {
    node next_node = NULL;
    if (cmp_func(a->data, b->data) > 0) {
      next_node = b;
      b         = b->next;
    } else {
      next_node = a;
      a         = a->next;
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

node _node_merge_sort(node head_ref, int (*cmp_func)(void *a, void *b)) {
  node middle_node, left_node, right_node;

  if (!head_ref || !(head_ref->next)) {
    return head_ref;
  }

  middle_node       = _node_get_middle(head_ref);
  left_node         = head_ref;
  right_node        = middle_node->next;
  middle_node->next = NULL;

  left_node  = _node_merge_sort(left_node, cmp_func);
  right_node = _node_merge_sort(right_node, cmp_func);

  return _merge_sorted_lists(left_node, right_node, cmp_func);
}

void linkedlist_merge_sort(linkedlist list, int (*cmp_func)(void *a, void *b)) {
  list->first = _node_merge_sort(list->first, cmp_func);
}

void linkedlist_iterate_over_nodes(linkedlist list, void (*callback)(node node_to_iterate)) {
  node current_node;

  current_node = list->first;

  while (current_node) {
    callback(current_node);
    current_node = current_node->next;
  }
}

data_t init_data(int *array, int len) {
  data_t r = malloc(sizeof(struct Data));

  r->array = array;
  r->len   = len;

  return r;
}

void free_data_t(data_t data) {
  free(data->array);
  free(data);
}

linkedlist list_from_array_of_data(data_t *data, int len) {
  linkedlist list;
  int        i;

  list = linkedlist_init();
  for (i = 0; i < len; i++) {
    linkedlist_append(list, data[i]);
  }

  return list;
}

int cmp_data(data_t data1, data_t data2) {
  int i;

  if (data1->len > data2->len) {
    return 1;
  } else if (data1->len < data2->len) {
    return -1;
  }

  for (i = 0; i < data1->len; i++) {
    if (data1->array[i] > data2->array[i]) {
      return 1;
    } else if (data1->array[i] < data2->array[i]) {
      return -1;
    }
  }

  return 0;
}

int my_cmp(void *a, void *b) {
  return cmp_data((data_t)a, (data_t)b);
}

data_t *read_data_array(int n) {
  int i, j, s, *a;

  data_t *data_array = malloc(n * sizeof(struct Data));

  for (i = 0; i < n; i++) {
    scanf("%d", &s);

    a = (int *)malloc(s * sizeof(int));

    for (j = 0; j < s; j++) {
      scanf("%d", &a[j]);
    }

    data_array[i] = init_data(a, s);
  }

  return data_array;
}

void print_callback(node iter) {
  int i;

  printf("%d ", ((data_t)iter->data)->len);

  for (i = 0; i < ((data_t)iter->data)->len; i++) {
    printf("%d ", ((data_t)iter->data)->array[i]);
  }

  printf("\n");
}

void free_data_callback(node iter) {
  free_data_t((data_t)iter->data);
}

void print_list(linkedlist list) {
  linkedlist_iterate_over_nodes(list, print_callback);
}

void free_all_data(linkedlist list) {
  linkedlist_iterate_over_nodes(list, free_data_callback);
}

int main() {
  linkedlist list;
  data_t    *data_array;
  int        n, i;

  scanf("%d", &n);

  data_array = read_data_array(n);
  list       = list_from_array_of_data(data_array, n);

  printf("\n\n");

  linkedlist_merge_sort(list, my_cmp);
  print_list(list);

  free_all_data(list);
  linkedlist_free(list);
  free(data_array);
}