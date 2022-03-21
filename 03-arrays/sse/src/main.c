#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void swap_int(int *x, int *y) {
  int temp;
  temp = *x;
  *x   = *y;
  *y   = temp;
}

void selection_sort_iteration(int *const arr, const int len, const int last) {
  int i, min;

  min = last;

  for (i = last; i < len; i++) {
    if (arr[i] < arr[min]) {
      min = i;
    }
  }

  swap_int(&arr[last], &arr[min]);
}

void print_array(const int *const arr, const int len) {
  int i;

  for (i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }

  printf("\n");
}

int main() {
  int i, len, pos, *array;

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

  selection_sort_iteration(array, len, pos);
  print_array(array, len);

  return 0;
}
