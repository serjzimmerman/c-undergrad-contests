#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef int (*cmp_t)(const void *lhs, const void *rhs);

void *cbsearch(const void *key, const void *base, int num, int size, cmp_t cmp) {
  char *low, *high, *mid;
  int r;

  low = (char *)base;
  high = low + (num - 1) * size;

  while (high != low) {
    mid = low + ((high - low) / size + 1) / 2 * size;
    
    r = cmp(mid, key);

    if (r == 0) {
      return mid;
    } else if (r > 0) {
      high = mid - size;
    } else {
      low = mid;
    }
  }

  if (cmp(low, key) == 0) {
    return low;
  }

  return NULL;
}

int cmp_int(const void *x, const void *y) {
  return *(int *)x - *(int *)y;
}

int main() {
  int array[] = {2, 2, 3, 4, 5, 6, 7, 7};
  int s = 2, *r;

  cmp_t cmp_i = &cmp_int;

  r = (int *)cbsearch(&s, &array, sizeof(array) / sizeof(int), sizeof(int), cmp_i);
  printf("%d\n", *r);
}
