#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int data;
};

struct node_t *node_init(int data) {
  struct node_t *node;

  node = calloc(1, sizeof(struct node_t));
  assert(node);

  node->data = data;

  return node;
}

int list_is_a_loop(struct node_t *top) {
  struct node_t *slow, *fast;

  slow = top;
  fast = top;

  if (top->next == top) {
    return 1;
  }

  while (fast && fast->next) {
    fast = fast->next->next;
    slow = slow->next;

    if (fast == slow) {
      return 1;
    }
  }

  return 0;
}

int main() {
  struct node_t *top;

  top = node_init(0);
  top->next = node_init(0);

  top->next->next = top->next;

  printf("%d\n", list_is_a_loop(top));
}
