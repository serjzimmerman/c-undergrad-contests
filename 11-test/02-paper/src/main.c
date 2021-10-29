#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int paper(int n, int *array, int m) {
  int i, index, max_index = 0;

  for (index = 1; index <= n; index++) {
    for (i = 0; i < index; i++) {
      if (array[i] < index) {
        if (index - array[i] <= m) {
          m -= (index - array[i]);
          array[i] = index;
        } else {
          break;
        }
      }
      if (i == index - 1) {
        max_index = index;
      }
    }
  }

  return max_index;
}

int main() {
  int i, n, m, *array;

  scanf("%d %d", &m, &n);

  array = malloc(sizeof(int) * n);

  for (i = 0; i < n; i++) {
    scanf("%d", &array[i]);
  }

  printf("%d\n", paper(n, array, m));

  free(array);
  return 0;
}
