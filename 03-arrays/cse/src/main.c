#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef int (*cmp_t)(const void *lhs, const void *rhs);

void swap(void *x, void *y, int eltsize) {
  char temp;
  int i;

  for (i = 0; i < eltsize; i++) {
    temp = ((char *)y)[i];
    ((char *)y)[i] = ((char *)x)[i];
    ((char *)x)[i] = temp;
  }
}

int selstep(void *parr, int eltsize, int numelts, int nsorted, cmp_t cmp) {
  char *i, *min, *last;

  min = last = (char *)parr + nsorted  * eltsize;

  for (i = last; i - (char *)parr < numelts * eltsize; i += eltsize) {
    if (cmp(i, min) == 1) {
      min = i;
    }
  }
  if (last != min) {
    swap(last, min, eltsize);
  }

  return 0;
}

int cmp_int(const void *x, const void *y) {
  return (*(int *)x < *(int *)y ? 1 : 0);
}

void print_array(const int * const arr, const int len) {
  int i;

  for (i = 0; i < len; i++) {
    if (i % 3 == 0) {
      printf("\n");
    }
    printf("%d ", arr[i]);
  }

  printf("\n");
}

int main() {
  int i, len, pos, *array;

  cmp_t cmp_i = cmp_int;

  if (scanf("%d", &len) != 1) {
    abort();
  }

  array = malloc(sizeof(int) * len);

  for (i = 0; i < len; i++) {
    scanf("%d", &array[i]);
  }

  if (scanf("%d", &pos) != 1) {
    abort();
  }

  selstep(array, sizeof(int), len, pos, cmp_i);
  print_array(array, len);

  free(array);

  return 0;
}