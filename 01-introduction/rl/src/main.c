#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int iabs(int x) { return (x < 0) ? -x : x; }

int eu_mod(int x, int y) {
  int r;

  assert(y != 0);

  r = x % y;

  if (r < 0) {
    r += iabs(y);
  }

  return r;
}

int gcd(int x, int y) {
  int q;

  assert(y != 0);

  while (y != 0) {
    q = eu_mod(x, y);

    x = y;
    y = q;
  }

  return y;
}

int main() {
  int x, y, g, res;

  res = scanf("%d%d", &x, &y);

  if (res != 2 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

  g = gcd(x, y);

  printf("%d\n", g);

  return 0;
}
