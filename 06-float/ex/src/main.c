#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  float    f;
  unsigned x;

  scanf("%f", &f);
  memcpy(&x, &f, sizeof(float));
  x ^= 0x2AAAAAUL;
  memcpy(&f, &x, sizeof(float));

  printf("%.5f", f);
}
