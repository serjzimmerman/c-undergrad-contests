#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct sieve_t {
  int n;
  unsigned char *s;
};

char char_array_get_bit(char *s, int n) {
  return ((s[n / CHAR_BIT] >> (n % CHAR_BIT)) & 1);
}

void char_array_set_bit(char *s, int n) {
  s[n / CHAR_BIT] |= (1 << (n % CHAR_BIT));
}

void fill_sieve(struct sieve_t *sv) {
  int i, c;

  for (c = 2; c < sqrt(sv->n * CHAR_BIT); c++) {
    if (!char_array_get_bit(sv->s, c)) {
      for (i = 2 * c; i < sv->n * CHAR_BIT; i += c) {
        char_array_set_bit(sv->s, i);
      }
    }
  }
}

int is_prime(struct sieve_t *sv, unsigned n) {
  return !(char_array_get_bit(sv->s, n));
}

int nth_prime(struct sieve_t *sv, unsigned n) {
  int i = 2;

  while (n--) {
    while (char_array_get_bit(sv->s, i++)) {
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

void free_sieve(struct sieve_t *sv) {
  free(sv->s);

  sv->s = NULL;
  sv->n = 0;

  free(sv);
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

  free(r);

  return 0;
}
