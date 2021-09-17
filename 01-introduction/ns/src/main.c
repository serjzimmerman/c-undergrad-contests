#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

unsigned ipow(unsigned i, unsigned p) {
  unsigned r;

  r = 1;

  while (p > 0) {
    r *= i;
    p--;
  }

  return r;
}

char itoa(unsigned i) {
  assert(i < 10);

  return (char)i + '0';
}

char *itoa_base_n(char *dest, unsigned n, unsigned base) {
  unsigned i;

  i = 0;

  while (ipow(base, i) <= n) {
    i++;
  }

  dest[i--] = '\0';

  while (n != 0) {
    dest[i--] = itoa(n % base);
    n /= base;
  }

  return dest;
}

int main() {
  int x, y, res;
  char dest[128];

  res = scanf("%d%d", &x, &y);

  if (res != 2 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

  printf("%s\n", itoa_base_n(dest, x, y));

  return 0;
}
