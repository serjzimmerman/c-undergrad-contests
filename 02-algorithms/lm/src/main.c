#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define WTF_MIN 2

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

  while ((q = eu_mod(x, y)) != 0) {
    x = y;
    y = q;
  }

  return y;
}

int lcm(int x, int y) {
  assert(x);
  assert(y);

  return (x * y) / gcd(x, y);
}

int wtf_function(int n) {
  int min = WTF_MIN, acc = WTF_MIN;

  while (min < n) {
    acc = lcm(acc, ++min);
  }

  return acc;
}

int main() {
  int x, g, res;

  res = scanf("%d", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  g = wtf_function(x);

  printf("%d\n", g);

  return 0;
}
