#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

unsigned is_prime(unsigned n) {
  unsigned j;

  if (n == 2) {
    return 1;
  }
  if (n < 2 || !(n % 2)) {
    return 0;
  }

  for (j = 3; j * j <= n; j += 2) {
    if (!(n % j)) {
      return 0;
    }
  }

  return 1;
}

unsigned prime_n(unsigned n) {
  int i = 1;

  while (n--) {
    while (!is_prime(++i)) {
    }
  }

  return i;
}

int main() {
  unsigned x, g, res;

  res = scanf("%u", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  g = prime_n(x);

  printf("%u\n", g);

  return 0;
}
