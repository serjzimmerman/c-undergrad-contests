#include "counter.h"
#include "hashtable.h"

#undef NDEBUG

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned long sdbm(const char *str) {
  unsigned long hash = 0;
  int c;

  while (c = *str++)
    hash = c + (hash << 6) + (hash << 16) - hash;

  return hash;
}

long long getnc(char *dst, size_t n) {
  long long i, c;

  assert(dst);

  while ((c = getchar()) == '\n') {
  }

  for (i = 0; i < n && c != EOF; i++) {
    *(dst++) = c;
    c = getchar();
  }

  *dst = '\0';

  return i;
}

long long gettokn(char *src) {
  long long r = 1;

  assert(src);

  while ((src = strchr(src, ' '))) {
    r++;
    src++;
  }

  return r;
}

int main(int argc, char **argv) {
  struct counter_t *counter;
  char *buf, *tok;
  long long a, l, n, res;

  assert(scanf("%d %d", &a, &l) == 2);

  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  assert(getnc(buf, l) == l);

  n = gettokn(buf);

  counter = counter_init(NULL);

  tok = strtok(buf, " ");
  while (tok) {
    counter_item_add(counter, tok);
    tok = strtok(NULL, " ");
  }
  free(buf);

  assert(scanf("%d", &l) == 1);

  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  assert(getnc(buf, l) == l);

  n = gettokn(buf);

  tok = strtok(buf, " ");
  while (tok) {
    printf("%d", counter_item_get_count(counter, tok));
    tok = strtok(NULL, " ");
    if (tok) {
      printf(" ");
    }
  }

  fprintf(stderr, "\nCollisions: \t%d\nInserts: \t%d\nSize: \t\t%d\n",
          hash_table_get_collisions(counter_get_hashtable(counter)),
          hash_table_get_inserts(counter_get_hashtable(counter)), hash_table_get_size(counter_get_hashtable(counter)));
  counter_free(counter);

  free(buf);
}