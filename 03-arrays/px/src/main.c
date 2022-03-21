#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 2

#define MATRIX_MN_SIZE N

typedef struct {
  union {
    unsigned mn_2d[MATRIX_MN_SIZE][MATRIX_MN_SIZE];
    unsigned mn_1d[MATRIX_MN_SIZE * MATRIX_MN_SIZE];
  } mn_array;
} mNxN;

mNxN mn_identity() {
  unsigned i, j;
  mNxN     r;

  for (i = 0; i < MATRIX_MN_SIZE; i++) {
    for (j = 0; j < MATRIX_MN_SIZE; j++) {
      r.mn_array.mn_2d[i][j] = (i == j);
    }
  }

  return r;
}

mNxN mn_mult_mod(const mNxN x, const mNxN y, const unsigned m) {
  unsigned i, j, k, t;
  mNxN     r;

  for (i = 0; i < MATRIX_MN_SIZE; i++) {
    for (j = 0; j < MATRIX_MN_SIZE; j++) {
      t = 0;
      for (k = 0; k < MATRIX_MN_SIZE; k++) {
        t = (t + ((x.mn_array.mn_2d[i][k]) * (y.mn_array.mn_2d[k][j])) % m) % m;
      }
      r.mn_array.mn_2d[i][j] = t;
    }
  }

  return r;
}

mNxN mn_pow_mod(mNxN x, unsigned n, unsigned m) {
  mNxN i, r;

  assert(m > 1);

  r = (i = mn_identity());

  if (!n) {
    return i;
  }

  while (n > 0) {
    if (n % 2) {
      r = mn_mult_mod(r, x, m);
    }
    x = mn_mult_mod(x, x, m);
    n = n / 2;
  }

  return r;
}

void powNxN(unsigned (*A)[N], unsigned k, unsigned m) {
  mNxN a, r;
  int  i, j;

  for (i = 0; i < MATRIX_MN_SIZE; i++) {
    for (j = 0; j < MATRIX_MN_SIZE; j++) {
      a.mn_array.mn_2d[i][j] = A[i][j];
    }
  }

  r = mn_pow_mod(a, k, m);

  for (i = 0; i < MATRIX_MN_SIZE; i++) {
    for (j = 0; j < MATRIX_MN_SIZE; j++) {
      A[i][j] = r.mn_array.mn_2d[i][j];
    }
  }
}

int main() {
  int      res;
  unsigned i, n, m;
  mNxN     a, b;

  res = scanf("%u%u%u%u%u%u", &a.mn_array.mn_2d[0][0], &a.mn_array.mn_2d[0][1], &a.mn_array.mn_2d[1][0],
              &a.mn_array.mn_2d[1][1], &n, &m);

  if (res != 6) {
    printf("%s\n", "Wrong input");
    abort();
  }

  b = mn_pow_mod(a, n, m);

  for (i = 0; i < MATRIX_MN_SIZE * MATRIX_MN_SIZE; i++) {
    printf(((i != MATRIX_MN_SIZE * MATRIX_MN_SIZE - 1) ? "%u " : "%u\n"), b.mn_array.mn_1d[i]);
  }

  return 0;
}
