#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned powers_of_four(unsigned n, unsigned m) {
  unsigned i, r = 0;

  for (i = (1 << 2 * m); n > 0; i >>= 2) {
    while (n >= i) {
      n -= i;
      r++;
    }
  }

  return r;
}

int main() {
  printf("%u\n", powers_of_four(27, 2));
  return 0;
}
