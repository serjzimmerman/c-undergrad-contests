#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ULL_BIT 63

int cmp_bitwise(unsigned long long x, unsigned long long y) {
  int i, r = 0;

  for (i = 0; i < ULL_BIT; i++) {
    if (x & 1) {
      if (!(y & 1)) {
        r++;
      }
    }
    x >>= 1;
    y >>= 1;
  }

  return r;
}

int main() {
  int res;
  unsigned long long x, y;

  res = scanf("%llu %llu", &x, &y);

  if (res != 2) {
    printf("%s\n", "Wrong input");
    abort();
  }

  printf("%d\n", cmp_bitwise(x, y));
  return 0;
}
