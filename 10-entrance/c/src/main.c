#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 1000

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

void array_reverse(long int *array, int size) {
  int i;

  for (i = 0; i < size / 2; i++) {
    swap(&array[i], &array[size - 1 - i]);
  }
}

int main() {
  long int pos[BUFF_SIZE], neg[BUFF_SIZE], final[BUFF_SIZE], temp = 0;

  int n = 0, ne = 0, po = 0, i;

  scanf("%d", &n);

  for (i = 0; i < n; i++) {
    scanf("%ld", &temp);
    if (temp >= 0) {
      pos[po++] = temp;
    } else {
      neg[ne++] = temp;
    }
  }

  bubble_sort(pos, po);
  bubble_sort(neg, ne);

  array_reverse(neg, ne);

  memcpy(&final, &pos, po * sizeof(long int));
  memcpy(&final[po], &neg, ne * sizeof(long int));

  for (i = 0; i < n; i++) {
    printf("%ld ", final[i]);
  }

  return 0;
}
