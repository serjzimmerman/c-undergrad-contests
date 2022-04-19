#include <stdlib.h>

#include "error.h"
#include "memutil.h"
#include <string.h>

#include "lru.h"
#include "sllistc.h"

void *slow_get_page(size_t index) {
  static int zero = 0;
  return &zero;
}

int main() {
  int    i, res = 0;
  size_t m, n, p;
  lru_t *cache;

  res   = scanf("%lu %lu", &m, &n);
  cache = lru_init(256, sizeof(int), slow_get_page);

  for (i = 0; i < n; ++i) {
    scanf("%d", &p);
    lru_get(cache, p);
  }

  printf("Cache_hits: %lu", lru_get_hits(cache));
}