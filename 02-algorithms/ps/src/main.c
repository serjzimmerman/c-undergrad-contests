#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct sieve_t {
  int   n;
  char *s;
};

void fill_sieve(struct sieve_t *sv) {
  int i, c;

  for (c = 2; c < sqrt(sv->n); c++) {
    if (!sv->s[c]) {
      for (i = 2 * c; i < sv->n; i += c) {
        sv->s[i] = 1;
      }
    }
  }
}

int nth_prime(struct sieve_t *sv, int N) {
  int i = 2;

  while (N--) {
    while (sv->s[i++]) {
    }
  }

  return i - 1;
}

struct sieve_t *init_sieve(int n) {
  struct sieve_t *r;

  r = (struct sieve_t *)malloc(sizeof(struct sieve_t));

  r->n = n;
  r->s = (char *)calloc(1, sizeof(char) * n);

  return r;
}

int main() {
  int x, y, g, res, n;

  res = scanf("%d %d", &x, &y);

  if (res != 2) {
    printf("%s\n", "Wrong input");
    abort();
  }

  struct sieve_t *r;

  r = init_sieve(x);
  fill_sieve(r);

  n = nth_prime(r, y);

  /* for (g = 1; g < r->n; g++) {
    printf("%d: %d\n", g, r->s[g]);
  } */

  printf("%d\n", n);

  return 0;
}
