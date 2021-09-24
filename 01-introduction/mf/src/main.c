#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MATRIX_M2_SIZE 2

typedef struct {
  union {
    unsigned m2_2d[MATRIX_M2_SIZE][MATRIX_M2_SIZE];
    unsigned m2_1d[MATRIX_M2_SIZE * MATRIX_M2_SIZE];
  } m2_array;
} m2;

m2 m2_identity() {
  unsigned i, j;
  m2 r;

  for (i = 0; i < MATRIX_M2_SIZE; i++) {
    for (j = 0; j < MATRIX_M2_SIZE; j++) {
      r.m2_array.m2_2d[i][j] = (i == j);
    }
  }

  return r;
}

m2 m2_mult_mod(const m2 x, const m2 y, const unsigned m) {
  unsigned i, j, k, t;
  m2 r;

  for (i = 0; i < MATRIX_M2_SIZE; i++) {
    for (j = 0; j < MATRIX_M2_SIZE; j++) {
      t = 0;
      for (k = 0; k < MATRIX_M2_SIZE; k++) {
        t = (t + ((x.m2_array.m2_2d[i][k]) * (y.m2_array.m2_2d[k][j])) % m) % m;
      }
      r.m2_array.m2_2d[i][j] = t;
    }
  }

  return r;
}

m2 m2_pow_mod(m2 x, unsigned n, unsigned m) {
  m2 i, r;

  assert(m > 1);

  r = (i = m2_identity());

  if (!n) { return i; }

  while (n > 0) {
    if (n % 2) { r = m2_mult_mod(r, x, m); }
    x = m2_mult_mod(x, x, m);
    n = n / 2;
  }

  return r;
}

int main() {
  int res;
  unsigned i, n, m;
  m2 a, b;

  res = scanf("%u%u%u%u%u%u", &a.m2_array.m2_2d[0][0], &a.m2_array.m2_2d[0][1],
              &a.m2_array.m2_2d[1][0], &a.m2_array.m2_2d[1][1], &n, &m);

  if (res != 6) {
    printf("%s\n", "Wrong input");
    abort();
  }

  b = m2_pow_mod(a, n, m);

  for (i = 0; i < MATRIX_M2_SIZE * MATRIX_M2_SIZE; i++) {
    printf(((i != MATRIX_M2_SIZE * MATRIX_M2_SIZE - 1) ? "%u " : "%u\n"),
           b.m2_array.m2_1d[i]);
  }

  return 0;
}
