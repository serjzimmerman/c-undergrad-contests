#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct area {
  long long unsigned width;
  long long unsigned height;
};

int compare(const void *a, const void *b) {
  struct area *first, *second;
  long long unsigned a1, a2, p1, p2;

  first = (struct area *)a;
  second = (struct area *)b;

  a1 = first->width * first->height;
  a2 = second->width * second->height;

  p1 = first->width + first->height;
  p2 = second->width + second->height;

  if (a1 != a2) {
    return a2 - a1;
  }

  else {
    if (p1 != p2) {
      return p1 - p2;
    }
    return second->height - first->height;
  }

  return 0;
}

int main() {
  struct area *array;
  int n, i;

  scanf("%d", &n);

  array = calloc(n, sizeof(struct area));

  for (i = 0; i < n; i++) {
    scanf("%llu %llu", &(array[i].width), &(array[i].height));
  }

  qsort(array, n, sizeof(struct area), compare);

  for (i = 0; i < n; i++) {
    printf("%llu %llu\n", array[i].width, array[i].height);
  }

  free(array);

  return 0;
}
