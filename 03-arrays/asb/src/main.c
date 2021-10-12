#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int sb(unsigned char c) {
  int b = 0;

  while (c > 0) {
    c &= (c - 1);
    b++;
  }

  return b;
}

int arrpopcount(const unsigned char *parr, int len) {
  int b = 0;

  while (--len >= 0) {
    b += sb(parr[len]);
  }

  return b;
}

int main() {
  unsigned char parr[] = {26, 9, 1, 11, 217};

  printf("%d", arrpopcount(&parr[0], sizeof(parr) / sizeof(parr[0])));

  return 0;
}
