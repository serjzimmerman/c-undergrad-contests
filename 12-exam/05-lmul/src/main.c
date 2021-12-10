#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct _Decimal {
  /* a[0]*10^0 + a[1]*10^1 + ..+ a[n]*10^n */
  char *a;
  unsigned int n;
  unsigned int size;
};
typedef struct _Decimal Decimal;

Decimal *mult(const Decimal *a, unsigned int k) {
  Decimal *res;
  int i;

  res = calloc(1, sizeof(Decimal));
  res->a = calloc(a->size, sizeof(char));
  res->size = a->size;

  if (k == 0) {
    res->n = 0;
    return res;
  }

  int carry = 0, temp = 0;

  for (i = 0;; i++) {
    if (i == res->size) {
      res->a = realloc(res->a, (res->size *= 2) * sizeof(char));
    }

    if (i <= a->n) {
      temp = k * a->a[i] + carry;
      carry = temp / 10;
      res->a[i] = temp % 10;
    } else {
      temp = carry;
      carry = temp / 10;
      res->a[i] = temp % 10;
    }

    if (i > a->n && carry == 0) {
      break;
    }
  }

  res->n = i;

  return res;
}

int main() {
  Decimal *res;
  int i;

  Decimal *a = calloc(1, sizeof(Decimal));
  a->a = calloc(7, sizeof(char));

  a->a[0] = 9;
  a->a[1] = 9;
  a->a[2] = 9;
  a->a[3] = 9;
  a->a[4] = 9;
  a->a[5] = 9;

  a->n = 5;
  a->size = 6;

  res = mult(a, 2);

  for (i = res->n; i >= 0; i--) {
    printf("%d ", res->a[i]);
  }
  printf("\n");

  free(res);

  return 0;
}
