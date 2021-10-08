#include <assert.h>
#include <inttypes.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define USE_BUILTIN_CTZ
#define SIEVE_BIT 8
#define min(x, y) (x > y ? y : x)

struct sieve_t {
  int n;
  uint8_t *s;
};

uint8_t uint8_array_get_bit(uint8_t *s, uint64_t n) {
  return ((s[n / SIEVE_BIT] >> (n % SIEVE_BIT)) & 1);
}

void uint8_array_set_bit(uint8_t *s, uint64_t n) {
  s[n / SIEVE_BIT] |= (1 << (n % SIEVE_BIT));
}

void fill_sieve(struct sieve_t *sv) {
  int i, c;

  for (c = 2; c < sqrt(sv->n * SIEVE_BIT); c++) {
    if (!uint8_array_get_bit(sv->s, c)) {
      for (i = 2 * c; i < sv->n * SIEVE_BIT; i += c) {
        uint8_array_set_bit(sv->s, i);
      }
    }
  }
}

int is_prime(struct sieve_t *sv, uint64_t n) {
  return !(uint8_array_get_bit(sv->s, n));
}

struct sieve_t *init_sieve(int n) {
  struct sieve_t *r;

  r = (struct sieve_t *)malloc(sizeof(struct sieve_t));

  r->n = n;
  r->s = (uint8_t *)calloc(1, sizeof(uint8_t) * n);

  return r;
}

void swap_uint64(uint64_t *a, uint64_t *b) {
  uint64_t temp;
  temp = *a;
  *a = *b;
  *b = temp;
}

/* This code is shit, there is a builtin gcc function __inbuilt_ctz() */
#ifndef USE_BUILTIN_CTZ
uint8_t ctz_uint64(uint64_t a) {
  int c;

  /* Zero trailing zeros if number if odd */
  if (a & 0x1) {
    return 0;
  }

  /* Similar to binary search */
  else {
    c = 0;
    if (!(a & 0xffffffff)) {
      a >>= 32;
      c += 32;
    }
    if (!(a & 0xffff)) {
      a >>= 16;
      c += 16;
    }
    if (!(a & 0xff)) {
      a >>= 8;
      c += 8;
    }
    if (!(a & 0xf)) {
      a >>= 4;
      c += 4;
    }
    if (!(a & 0x3)) {
      a >>= 2;
      c += 2;
    }
    if (!(a & 0x1)) {
      c += 1;
    }
    return c;
  }
}
#endif

uint64_t binary_gcd(uint64_t a, uint64_t b) {
  uint8_t i, j;

  if (!a) {
    return b;
  } else if (!b) {
    return a;
  }

#ifdef USE_BUILTIN_CTZ
  i = __builtin_ctz(a);
  j = __builtin_ctz(b);
#else
  i = ctz_uint64(a);
  j = ctz_uint64(b);
#endif

  a >>= i;
  b >>= j;

  i = min(i, j);

  for (;;) {
    if (a > b) {
      swap_uint64(&a, &b);
    }

    b -= a;

    if (!b) {
      return a << i;
    }

#ifdef USE_BUILTIN_CTZ
    b >>= __builtin_ctz(b);
#else
    b = ctz_uint64(b);
#endif
  }

  return -1;
}

uint64_t mult_mod_uint64(uint64_t a, uint64_t n, uint64_t m) {
  uint64_t r;

  assert(m > 1);

  a = a % m;
  r = 0;

  if (!n) {
    return 0;
  }

  while (n > 0) {
    if (n & 0x1) {
      r = (r + a) % m;
    }
    a = (a << 1) % m;
    n >>= 1;
  }

  return r % m;
}

uint64_t pow_mod_uint64(uint64_t a, uint64_t n, uint64_t m) {
  uint64_t r;

  assert(m > 1);

  a = a % m;
  r = 1;

  if (!n) {
    return r;
  }

  while (n > 0) {
    if (n & 1) {
      r = mult_mod_uint64(a, r, m);
    }
    a = mult_mod_uint64(a, a, m);
    n >>= 1;
  }

  return r % m;
}

uint8_t fermat_primality_test(uint64_t p, uint32_t i) {
  uint32_t c;
  uint64_t a;

  for (c = 0; c < i; c++) {
    do {
      a = rand() % (p - 4) + 2;
    } while (binary_gcd(a, p) != 1);
    if (pow_mod_uint64(a, p - 1, p) != 1) {
      return 0;
    }
  }

  return 1;
}

#define SIEVE_SIZE 200000
#define MAX_SEARCH (1ull << 60)

uint64_t max_pf(uint8_t k, uint8_t n) {
  struct sieve_t *r;
  uint64_t max = 0, s = 1, s_prev = 1, s_prev_prev = 0, temp;

  s = k * s_prev + n * s_prev_prev;

  r = init_sieve(SIEVE_SIZE);
  fill_sieve(r);

  if (binary_gcd(k, n) != 1) {
    if (is_prime(r, s)) {
      return s;
    }
    return 0;
  }

  while (s < MAX_SEARCH) {
    if (s < SIEVE_SIZE * SIEVE_BIT) {
      if (is_prime(r, s)) {
        max = s;
      }
    } else if (fermat_primality_test(s, 200)) {
      max = s;
    }
    temp = s;
    s = k * s + n * s_prev;
    s_prev_prev = s_prev;
    s_prev = temp;
  }

  return max;
}

int main() {
  int res;
  uint8_t k, n;

  res = scanf("%hhu%hhu", &k, &n);

  if (res != 2) {
    printf("%s\n", "Wrong input");
    abort();
  }

  srand(time(NULL));

  printf("%" PRIu64 "\n", max_pf(k, n));

  return 0;
}
