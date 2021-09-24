#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define WTF_MIN 2l

long iabs(long x) { return (x < 0) ? -x : x; }

long eu_mod(long x, long y) {
  long r;

  assert(y != 0);

  r = x % y;

  if (r < 0) { r += iabs(y); }

  return r;
}

long gcd(long x, long y) {
  long q;

  assert(y != 0);

  while ((q = eu_mod(x, y)) != 0) {
    x = y;
    y = q;
  }

  return y;
}

long lcm(long x, long y) {
  assert(x);
  assert(y);

  return (x * y) / gcd(x, y);
}

long wtf_function(long n) {
  long min = WTF_MIN, acc = WTF_MIN;

  while (min < n) {
    acc = lcm(acc, ++min);
  }

  return acc;
}

int main() {
  long int x, g, res;

  res = scanf("%ld", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  g = wtf_function(x);

  printf("%ld\n", g);

  return 0;
}
