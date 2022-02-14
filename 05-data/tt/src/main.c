#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHAR_BIT 8

void char_array_set_bit(char *array, unsigned bit, unsigned state) {
  array[bit / CHAR_BIT] = (array[bit / CHAR_BIT] & ~(1 << (bit % CHAR_BIT))) | (state << (bit % CHAR_BIT));
}

unsigned char_array_get_bit(char *array, unsigned bit) {
  return array[bit / CHAR_BIT] & (1 << (bit % CHAR_BIT));
}

struct tnode_t {
  struct tnode_t *left, *right;
  int value;
};

struct tbinary_t {
  char *char_array;
  unsigned size_char_array, size_int_array, char_len, int_len;
  int *int_array;
};

struct tbinary_t *node_binary_init(unsigned size) {
  struct tbinary_t *binary;

  binary = calloc(1, sizeof(struct tbinary_t));

  if (!binary) {
    return NULL;
  }

  binary->size_char_array = size;
  binary->size_int_array = size;

  binary->char_len = 0;
  binary->int_len = 0;

  binary->char_array = calloc(size, sizeof(char));

  if (!binary->char_array) {
    free(binary);
    return NULL;
  }

  binary->int_array = calloc(size, sizeof(int));

  if (!binary->char_array) {
    free(binary);
    free(binary->char_array);
    return NULL;
  }

  return binary;
}

void node_binary_free(struct tbinary_t *binary) {
  free(binary->char_array);
  free(binary->int_array);
  free(binary);
}

void node_binary_append(struct tbinary_t *binary, unsigned a, int *v) {
  assert(binary);

  if (binary->size_char_array * 8 == binary->char_len) {
    binary->char_array = realloc(binary->char_array, (binary->size_char_array *= 2) * sizeof(char));
    if (!binary->char_array) {
      fprintf(stderr, "Unable to allocate memory\n");
      exit(EXIT_FAILURE);
    }
  }

  if (binary->size_int_array == binary->int_len) {
    binary->int_array = realloc(binary->int_array, (binary->size_int_array *= 2) * sizeof(int));
    if (!binary->int_array) {
      fprintf(stderr, "Unable to allocate memory\n");
      exit(EXIT_FAILURE);
    }
  }

  char_array_set_bit(binary->char_array, binary->char_len++, (a ? 1 : 0));

  if (v) {
    binary->int_array[binary->int_len++] = *v;
  }
}

void node_binary_print(struct tbinary_t *binary) {
  unsigned i;

  for (i = 0; i < binary->char_len - 1; i++) {
    printf(char_array_get_bit(binary->char_array, i) ? "1 " : "0 ");
  }

  for (i = 0; i < binary->int_len; i++) {
    printf("%d ", binary->int_array[i]);
  }
}

struct tnode_t *node_init(struct tnode_t *left, struct tnode_t *right, int value) {
  struct tnode_t *node;

  node = calloc(1, sizeof(struct tnode_t));

  if (!node) {
    return NULL;
  }

  node->left = left;
  node->right = right;
  node->value = value;

  return node;
}

void node_set_left(struct tnode_t *node, struct tnode_t *left) {
  assert(node);

  node->left = left;
}

void node_set_right(struct tnode_t *node, struct tnode_t *right) {
  assert(node);

  node->right = right;
}

void get_node_binary(struct tnode_t *node, struct tbinary_t *binary) {
  node_binary_append(binary, 1, &node->value);
  if (node->left) {
    get_node_binary(node->left, binary);
  } else {
    node_binary_append(binary, 0, NULL);
  }

  if (node->right) {
    get_node_binary(node->right, binary);
  } else {
    node_binary_append(binary, 0, NULL);
  }
}

struct tnode_t *tree_generate_from_traverse(int *inorder, unsigned inl, int *preorder, unsigned prel) {
}

int main() {
  struct tnode_t *a = node_init(NULL, NULL, 1);
  struct tnode_t *b = node_init(NULL, NULL, 2);
  struct tnode_t *c = node_init(a, NULL, 3);
  struct tnode_t *d = node_init(b, NULL, 0);
  struct tnode_t *r = node_init(c, d, 4);

  struct tbinary_t *binary = node_binary_init(1);

  get_node_binary(r, binary);
  node_binary_print(binary);
  node_binary_free(binary);

  free(a);
  free(b);
  free(c);
  free(d);
  free(r);
}
