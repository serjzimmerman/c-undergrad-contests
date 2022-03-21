#include <assert.h>
#include <stdio.h>

int majority_element(const int *parr, int len) {
  int middle, m1, m2, i, j, counter;

  assert(parr);

  if (len == 1) {
    return parr[0];
  }

  middle = len / 2;

  m1 = majority_element(&parr[0], middle);
  m2 = majority_element(&parr[middle], len - middle);

  if (m1 == m2) {
    return m1;
  }

  i = j   = 0;
  counter = len;
  while (--counter >= 0) {
    if (parr[counter] == m1) {
      i++;
    } else if (parr[counter] == m2) {
      j++;
    }
  }

  if (i <= len / 2 && j <= len / 2) {
    return -1;
  }

  return (i > j ? m1 : m2);
}

int main() {
  int array[] = {3, 3, 4, 2, 4, 4, 2, 4};

  printf("%d\n", majority_element(&array[0], sizeof(array) / sizeof(array[0])));

  return 0;
}
