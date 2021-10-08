#include <assert.h>
#include <limits.h>
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct sieve_t {
  int n;
  char *s;
};

int ipow(int n, int p) {
  int r = 1;

  assert(n);

  while (p-- > 0) {
    r *= n;
  }

  return r;
}

char itoa_c(i) {
  assert(i < 10);

  return (char)i + '0';
}

char *itoa_base_n(char *dest, int n, int base) {
  int i;

  i = 0;

  while (ipow(base, i) <= n) {
    i++;
  }

  dest[i--] = '\0';

  while (n != 0) {
    dest[i--] = itoa_c(n % base);
    n /= base;
  }

  return dest;
}

int shift_right_int(int n, int m) {
  char s[16] = "", d[8] = "";
  int len;

  assert(m < n);

  itoa_base_n(d, n, 10);
  len = strlen(d);

  strncpy(s, d, strlen(d));
  strncat(s, d, strlen(d));

  strncpy(d, &s[len - m - 1], len);

  return atoi(d);
}

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

int is_prime(struct sieve_t *sv, int n) {
  return !(char_array_get_bit(sv->s, n));
}

int nth_prime(struct sieve_t *sv, int n) {
  int i = 2;

  while (n--) {
    while (char_array_get_bit(sv->s, i++)) {
    }
  }

  return i - 1;
}

int is_circular_prime(struct sieve_t *sv, int n) {
  int i = 10, r = 1, temp = n;

  while (i <= n) {
    r++;
    i *= 10;
  }

  for (i = 0; i < r; i++) {
    /* printf("\tn = %d, i = %d\n", temp, i); */
    if (!is_prime(sv, temp)) {
      return 0;
    }
    temp = shift_right_int(n, i);
  }

  return 1;
}

int closest_circular_prime(struct sieve_t *sv, int n) {
  int pos = n, neg = n, is_pos, is_neg;
  for (;;) {
    if (pos < (sv->n * CHAR_BIT - 1)) {
      if ((is_pos = is_circular_prime(sv, ++pos))) {
        /* printf("pos = %d, is circular = %d", pos, is_pos); */
        return pos;
      }
    }
    if ((is_neg = is_circular_prime(sv, --neg))) {
      /* printf("neg = %d, is circular = %d", neg, is_neg); */
      return neg;
    }
  }
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
  int x, res;
  struct sieve_t *r;

  res = scanf("%d", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  r = init_sieve(125000);
  fill_sieve(r);

  printf("%d\n", closest_circular_prime(r, x));

  free_sieve(r);
  return 0;
}
