#include "counter.h"
#include "hashtable.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void getnc(char *dst, size_t n) {
  int i;
  char c;

  assert(dst);

  while ((c = getchar()) == '\n') {
  }

  for (i = 0; i < n; i++) {
    *(dst++) = c;
    c = getchar();
  }

  *dst = '\0';
}

int gettokn(char *src) {
  int r = 1;

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
  int a, l, n;

  scanf("%d %d", &a, &l);

  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  getnc(buf, l);
  n = gettokn(buf);

  counter = counter_init();

  tok = strtok(buf, " ");
  while (tok) {
    counter_item_add(counter, tok);
    tok = strtok(NULL, " ");
  }
  free(buf);

  scanf("%d", &l);
  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  getnc(buf, l);
  n = gettokn(buf);

  tok = strtok(buf, " ");
  while (tok) {
    printf("%d", counter_item_get_count(counter, tok));
    tok = strtok(NULL, " ");
    if (tok) {
      printf(" ");
    }
  }

#ifdef HASH_TABLE_USE_COLLISIONS
  printf("Hash collisions: %d\n", counter->table->collisions);
#endif
  counter_free(counter);

  free(buf);
}