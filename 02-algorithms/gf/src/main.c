#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct sieve_t {
  int n;
  char *s;
};

struct sequence_t {
  int a, b, n;
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

struct sequence_t get_largest_sequence(struct sieve_t *sv, int n) {
  struct sequence_t current_sequence = {0}, max_sequence = {0};
  int temp;

  for (current_sequence.a = -n + 1; current_sequence.a < n;
       current_sequence.a++) {
    for (current_sequence.b = -n + 1; current_sequence.b < n;
         current_sequence.b++) {
      current_sequence.n = 0;
      while ((temp = current_sequence.n * current_sequence.n +
                     current_sequence.a * current_sequence.n +
                     current_sequence.b) > 0 &&
             is_prime(sv, temp)) {
        current_sequence.n++;
      }
      if (current_sequence.n > max_sequence.n) {
        max_sequence.n = current_sequence.n;
        max_sequence.a = current_sequence.a;
        max_sequence.b = current_sequence.b;
      }
    }
  }

  return max_sequence;
}

int main() {
  int x, res;
  struct sieve_t *r;
  struct sequence_t max;

  res = scanf("%d", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  r = init_sieve(10000000);
  fill_sieve(r);

  max = get_largest_sequence(r, x);

  /* for (g = 1; g < r->n; g++) {
    printf("%d: %d\n", g, r->s[g]);
  } */

  printf("%d %d %d\n", max.a, max.b, max.n);

  free_sieve(r);
  return 0;
}
