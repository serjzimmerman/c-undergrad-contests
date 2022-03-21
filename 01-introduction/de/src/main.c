#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define DIOPHANTINE_SOL_NONE -1
#define DIOPHANTINE_SOL_INF 1

struct ExtendedGcd {
  int a, b, g;
};

struct DiophantineSolution {
  int sol_count;
  int sol_array[2];
};

void swap(int *x, int *y) {
  int temp;

  temp = *x;
  *x   = *y;
  *y   = temp;
}

int iabs(const int x) {
  return (x < 0) ? -x : x;
}

int eu_mod(int x, int y) {
  int r;

  assert(y != 0);

  r = x % y;

  if (r < 0) {
    r += iabs(y);
  }

  return r;
}

int eu_div(const int x, const int y) {
  assert(y != 0);

  return (x - eu_mod(x, y)) / y;
}

/* https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm */
int extended_gcd(struct ExtendedGcd *egcd, const int x, const int y) {
  int old_r, r, old_s, s, old_t, t, q;

  old_r = x;
  old_s = 1;
  old_t = 0;
  r     = y;
  s     = 0;
  t     = 1;

  assert(y != 0);

  while (r != 0) {
    q = eu_div(old_r, r);
    swap(&old_r, &r);
    swap(&old_s, &s);
    swap(&old_t, &t);
    r = r - q * old_r;
    s = s - q * old_s;
    t = t - q * old_t;
  }

  egcd->g = old_r;
  egcd->a = old_s;
  egcd->b = old_t;

  return 0;
}

int solve_diophantine(struct DiophantineSolution *dsol, const int x, const int y, const int c) {
  int                k;
  struct ExtendedGcd egcd;

  extended_gcd(&egcd, x, y);

  if (c % egcd.g != 0) {
    dsol->sol_count = DIOPHANTINE_SOL_NONE;
    return 0;
  }

  k = c / egcd.g;

  dsol->sol_array[0] = k * egcd.a;
  dsol->sol_array[1] = k * egcd.b;
  dsol->sol_count    = 2;

  return 0;
}

int main() {
  int                        x, y, c, res;
  struct DiophantineSolution dsol;

  res = scanf("%d%d%d", &x, &y, &c);

  if (res != 3 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

  solve_diophantine(&dsol, x, y, c);

  if (dsol.sol_count == DIOPHANTINE_SOL_NONE) {
    printf("%s\n", "NONE\n");
  } else {
    printf("%d %d\n", dsol.sol_array[0], dsol.sol_array[1]);
  }

  return 0;
}
