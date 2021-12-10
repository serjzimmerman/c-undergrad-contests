#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct node_t {
  struct node_t *next;
  char *name;
};

void node_free(struct node_t *node) {
  free(node->name);
  free(node);
}

struct node_t *node_init(char *name) {
  struct node_t *node;

  node = calloc(1, sizeof(struct node_t));
  node->name = name;

  return node;
}

struct node_t *path_create(char *path) {
  struct node_t *node;
  char *next_slash, *name, *current;
  int len;

  next_slash = strchr(path, '/');

  if (next_slash == NULL) {
    len = strlen(path);
  } else {
    len = next_slash - path;
  }

  name = calloc(len + 1, sizeof(char));
  memcpy(name, path, len);

  node = node_init(name);

  if (next_slash == NULL) {
    node->next = NULL;
  } else {
    node->next = path_create(next_slash + 1);
  }

  return node;
}

/* Very error-prone code, possible segfault */
struct node_t *path_normalize(struct node_t *node) {
  struct node_t *next;

  if (node->next == NULL) {
    if (strcmp(node->name, "") == 0 || strcmp(node->name, ".") == 0) {
      node_free(node);
      return NULL;
    }
    return node;
  }

  else if (node->next != NULL) {
    if (strcmp(node->next->name, "..") == 0) {
      next = node->next;
      node_free(node);
      node = next;
      next = node->next;
      node_free(node);
      return path_normalize(next);
    }
    if (strcmp(node->name, "") == 0 || strcmp(node->name, ".") == 0) {
      next = node->next;

      node_free(node);

      return path_normalize(next);
    }
  }

  node->next = path_normalize(node->next);
  return node;
}

void print_path_to_str(struct node_t *head, char *str) {
  struct node_t *node;
  int len;

  node = head;

  len = strlen(str);
  memset(str, 0, len);

  while (node) {
    strcat(str, node->name);
    if (node->next) {
      strcat(str, "/");
    }

    node = node->next;
  }
}

void normalize_path(char *path) {
  struct node_t *head, *next, *temp;

  head = path_create(path);
  head = path_normalize(head);

  if (*path == '/') {
    print_path_to_str(head, path + 1);
  } else {
    print_path_to_str(head, path);
  }

  while (head) {
    next = head->next;
    node_free(head);
    head = next;
  }
}

int main() {
  char *str = "/var/log/..////../lib/./ejexec";

  char *buf = calloc(100, sizeof(char));
  strcat(buf, str);

  normalize_path(buf);

  printf("%s\n", buf);
}
