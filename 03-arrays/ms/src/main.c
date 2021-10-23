#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void merge(int *arr, int l, int m, int r);

void merge_sort_imp(int *arr, int l, int r) {
  int m;

  if (l >= r) {
    return;
  }

  m = (l + r) / 2;

  merge_sort_imp(arr, l, m);
  merge_sort_imp(arr, m + 1, r);

  merge(arr, l, m, r);
}

void merge_sort(int *arr, int n) {
  merge_sort_imp(arr, 0, n - 1);
}

void merge(int *arr, int l, int m, int r) {
  int i, j, c, *t;

  t = malloc((r - l + 1) * sizeof(int));

  i = l;
  j = m + 1;
  c = 0;

  while (i <= m && j <= r) {
    if (arr[i] < arr[j]) {
      t[c++] = arr[i++];
    } else {
      t[c++] = arr[j++];
    }
  }

  while (i <= m) {
    t[c++] = arr[i++];
  }

  while (j <= r) {
    t[c++] = arr[j++];
  }

  for (c = 0; c < (r - l + 1); c++) {
    arr[l + c] = t[c];
  }

  free(t);
}

void print_array(const int *const arr, const int len) {
  int i;

  for (i = 0; i < len; i++) {
    printf("%d ", arr[i]);
  }

  printf("\n");
}

int main() {
  int i, len, *array;

  if (scanf("%d", &len) != 1) {
    abort();
  }

  array = malloc(sizeof(int) * len);

  for (i = 0; i < len; i++) {
    scanf("%d", &array[i]);
  }

  merge_sort(array, len);
  print_array(array, len);

  return 0;
}
