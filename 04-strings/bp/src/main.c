#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int int_cmp(const unsigned *x, const unsigned *y) {
  return ((int)(*x) - (int)(*y));
}

unsigned max_items(unsigned capacity, unsigned n, unsigned *array) {
  int i;

  qsort(array, n, sizeof(unsigned), (int (*)(void const *, void const *))int_cmp);

  for (i = 0; i < n; i++) {
    if (capacity < array[i]) {
      return i;
    }
    capacity -= array[i];
  }

  return i;
}

/* It is possible to solve this problem using a greedy algorithm without dynamic programming (because all items have the
 * same value)*/

int main() {
  unsigned c, n, *array;
  int i;

  scanf("%u %u", &c, &n);

  array = malloc(n * sizeof(unsigned));

  for (i = 0; i < n; i++) {
    scanf("%u", &array[i]);
  }

  printf("%u\n", max_items(c, n, array));

  free(array);
  return 0;
}
