#include <stdlib.h>

#include "error.h"
#include "memutil.h"
#include <string.h>

#include "lru.h"
void *slow_get_page(size_t index) {
  static int zero = 0;
  return &zero;
}

int main() {
  int    i, res = 0;
  size_t m, n, p = 0;
  lru_t *cache = NULL;

  res = scanf("%lu %lu", &m, &n);
  if (!res) {
    ERROR("Invalid input\n");
  }

  cache = lru_init(m, sizeof(int), slow_get_page);

  for (i = 0; i < n; ++i) {
    scanf("%lu", &p);
    lru_get(cache, p);
  }

  printf("%lu", lru_get_hits(cache));
  lru_free(cache);
}