#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

// #define USE_BUILTIN_CLZ

int msb(unsigned long long a) {
  int c = 0;

  if (!a) {
    return -1;
  }

#ifdef USE_BUILTIN_CLZ
  return 63 - __builtin_clzll(a);
#else
  while (a >>= 1) {
    c++;
  }
  return c;
#endif
}

int main() {
  int res;
  unsigned long long x;

  res = scanf("%llu", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  printf("%d", msb(x));

  return 0;
}
