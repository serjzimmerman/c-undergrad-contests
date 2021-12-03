#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  int data;
};

struct node_t *init_node(int val) {
  struct node_t *node;

  node = calloc(1, sizeof(struct node_t));
  node->data = val;

  return node;
}

struct node_t *reverse(struct node_t *top) {
  struct node_t *prev, *current, *next;

  prev = NULL;
  next = NULL;

  current = top;

  while (current) {
    next = current->next;
    current->next = prev;

    prev = current;
    current = next;
  }

  return prev;
}

struct node_t *read_list(FILE *inp) {
  struct node_t *temp_node, dummy_even = {0}, dummy_odd = {0}, *current_even,
                            *current_odd;
  int result, temp;

  current_even = &dummy_even;
  current_odd = &dummy_odd;

  while ((result = fscanf(inp, "%d", &temp)) != EOF && result == 1) {
    temp_node = init_node(temp);
    if (temp & 1) {
      current_odd->next = temp_node;
      current_odd = temp_node;
    } else {
      current_even->next = temp_node;
      current_even = temp_node;
    }
  }

  if (dummy_odd.next) {
    current_even->next = dummy_odd.next;
  }

  return dummy_even.next;
}

void delete_list(struct node_t *top) {
  struct node_t *current, *next;

  if (!top) {
    return;
  }

  current = top;
  next = top->next;

  while (current) {
    free(current);

    current = next;
    if (next) {
      next = current->next;
    }
  }
}

void print_list(struct node_t *top) {
  struct node_t *current;

  current = top;
  while (current) {
    printf("%d -> ", current->data);
    current = current->next;
  }
  printf("\n");
}

int main() {
  struct node_t *list;
  FILE *fp;

  fp = fopen("test.txt", "r");
  list = read_list(fp);

  print_list(list);

  list = reverse(list);
  print_list(list);

  delete_list(list);

  fclose(fp);
  return 0;
}
