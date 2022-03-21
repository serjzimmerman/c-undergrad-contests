#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct tree_t {
  struct tree_t *left, *right;
  int            data;
};

/* TODO: write supporting code */

struct tree_t *tree_left_most(struct tree_t *root) {
  while (root->left) {
    root = root->left;
  }
  return root;
}

struct tree_t *tree_right_most(struct tree_t *root) {
  while (root->right) {
    root = root->right;
  }
  return root;
}

/* O(n * log(n)) */

int check_is_bst(struct tree_t *top) {
  if (!top || (!top->left && !top->right)) {
    return 1;
  }

  if (top->right && top->data > tree_left_most(top->right)->data) {
    return 0;
  }

  if (top->left && top->data < tree_right_most(top->left)->data) {
    return 0;
  }

  return (check_is_bst(top->left) && check_is_bst(top->right));
}

int main() {
}