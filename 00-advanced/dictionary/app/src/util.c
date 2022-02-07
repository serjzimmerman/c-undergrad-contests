#include "util.h"
#include "hashtable.h"
#include "sllistc.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

char *stolower(char *src) {
  char *old = src;

  while (*src != '\0') {
    *src = tolower(*src);
    src++;
  }

  return old;
}

int pair_cmp(void *a, void *b) {
  return (pair_get_value((struct pair_t *)a) - pair_get_value((struct pair_t *)b));
}

void print_callback(struct sl_node_t *node, struct sl_list_t *list, va_list argp) {
  struct pair_t *pair;
  FILE *fs;

  fs = va_arg(argp, FILE *);
  pair = sl_node_get_data(node);

  fprintf(fs, "%s -- %u\n", pair_get_key(pair), pair_get_value(pair));
}

void sl_list_print(struct sl_list_t *list, FILE *fs) {
  sl_list_iterate_over_nodes(list, print_callback, fs);
}