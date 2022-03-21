#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int            data;
};

struct node_t *node_init(int data) {
  struct node_t *node;

  node = calloc(1, sizeof(struct node_t));
  assert(node);

  node->data = data;

  return node;
}

int loop_len(struct node_t *top) {
  struct node_t *slow, *fast;
  int            l = 0;

  if (!top) {
    return 0;
  }

  slow = top;
  fast = top;

  if (top->next == top) {
    return 1;
  }

  for (;;) {
    if (!fast || !fast->next) {
      return 0;
    }

    fast = fast->next->next;
    slow = slow->next;

    if (fast == slow) {
      break;
    }
  }

  slow = top;

  while (slow != fast) {
    slow = slow->next;
    fast = fast->next;
  }

  for (;;) {
    slow = slow->next;
    l    = l + 1;
    if (slow == fast) {
      return l;
    }
  }
}

int main() {
  struct node_t *top;

  top       = node_init(0);
  top->next = top;

  printf("%d\n", loop_len(top));
}
