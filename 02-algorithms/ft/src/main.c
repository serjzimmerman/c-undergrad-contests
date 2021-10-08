#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define USE_BUILTIN_CTZ

#define min(x, y) (x > y ? y : x)

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
    a = rand() % (p - 4) + 2;
    if (binary_gcd(a, p) == 1 && pow_mod_uint64(a, p - 1, p) != 1) {
      return 0;
    }
  }

  return 1;
}

int main() {
  int res;
  uint64_t x;

  res = scanf("%" PRIu64, &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  srand(time(NULL));

  printf("%d\n", fermat_primality_test(x, 10000));

  return 0;
}
