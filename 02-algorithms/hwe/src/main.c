#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#define SIEVE_BIT 8

struct sieve_t {
  int n;
  unsigned char *mod1;
  unsigned char *mod5;
};

unsigned char char_array_get_bit(unsigned char *s, int n) {
  return ((s[n / SIEVE_BIT] >> (n % SIEVE_BIT)) & 1);
}

void char_array_set_bit(unsigned char *s, int n) {
  s[n / SIEVE_BIT] |= (1 << (n % SIEVE_BIT));
}

void sieve_set_number(struct sieve_t *sv, unsigned i) {
  int mod, n;

  mod = i % 6;
  n = (i - mod) / 6;

  if (mod != 1 || mod != 5) {
    return;
  }

  char_array_set_bit(((mod == 1) ? sv->mod1 : sv->mod5), n);
}

int sieve_get_number(struct sieve_t *sv, unsigned i) {
  int mod, n;

  mod = i % 6;
  n = (i - mod) / 6;

  return char_array_get_bit(((mod == 1) ? sv->mod1 : sv->mod5), n);
}

void fill_sieve(struct sieve_t *sv) {
  int i, c;

  for (c = 2; c * c <= (sv->n * SIEVE_BIT) * 6 + 5; c++) {
    if (!sieve_get_number(sv, c)) {
      for (i = 2 * c; i < (sv->n * SIEVE_BIT) * 6 + 5; i += c) {
        sieve_set_number(sv, i);
      }
    }
  }
}

int is_prime(struct sieve_t *sv, unsigned n) {
  if (n == 2 || n == 3 || n == 5) {
    return 1;
  }

  return !sieve_get_number(sv, n);
}

int main() {
  struct sieve_t sieve = {0};

  sieve.n = 2;
  sieve.mod1 = calloc(sieve.n, sizeof(unsigned char));
  sieve.mod5 = calloc(sieve.n, sizeof(unsigned char));

  fill_sieve(&sieve);

  for (int i = 0; i < 128; i++) {
    if (is_prime(&sieve, i)) {
      printf("%d\n", i);
    }
  }
  return 0;
}
