#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void swap(long int *x, long int *y) {
  long int temp;
  temp = *x;
  *x   = *y;
  *y   = temp;
}

void bubble_sort(long int *array, int size) {
  int i, j;

  for (i = 0; i < size - 1; i++) {
    for (j = 0; j < size - 1 - i; j++) {
      if (array[j] > array[j + 1]) {
        swap(&array[j], &array[j + 1]);
      }
    }
  }
}

int main() {
  long int temp[3] = {0};

  scanf("%ld %ld %ld", &temp[0], &temp[1], &temp[2]);
  bubble_sort(temp, 3);
  printf("%ld\n", labs(temp[0] - temp[2]));

  return 0;
}
