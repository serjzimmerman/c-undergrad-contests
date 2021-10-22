#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int moveright(int *arr, int key, int last) {
  int i, j;
  for (j = 0; arr[j] <= key && j < last; j++) {
  }
  for (i = last; i > j; i--) {
    arr[i] = arr[i - 1];
  }
  return j;
}

void inssort(int *arr, int len) {
  int i;
  for (i = 0; i < len; ++i) {
    int key, pos;
    key = arr[i];
    pos = moveright(arr, key, i);
    arr[pos] = key;
  }
}

int main() {
  int array[] = {5, 3, 2, 100, -1, -1, 5, 2, 5, 4, 10}, i;

  inssort(array, sizeof(array) / sizeof(int));

  for (i = 0; i < sizeof(array) / sizeof(int); i++) {
    printf("%d\n", array[i]);
  }

  return 0;
}
