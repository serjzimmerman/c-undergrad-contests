#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

unsigned long long spiral(int n) {
  unsigned long long t, i, r;
  if (n == 1) {
    return 1;
  }
  t = r = n * n;
  for (i = 1; i < 4; i++) {
    t -= (n - 1);
    r += t;
  }
  return r + spiral(n - 2);
}

int main() {
  int n;

  scanf("%d", &n);

  printf("%llu\n", spiral(n));

  return 0;
}
