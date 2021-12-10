#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 128

struct _Decimal {
  /* a[0]*10^0 + a[1]*10^1 + ..+ a[n]*10^n */
  char a[N];
  unsigned int n;
};
typedef struct _Decimal Decimal;

int is_equal(Decimal *a, Decimal *b) {
  int i;

  if (a->n != b->n) {
    return 0;
  }

  for (i = 0; i <= a->n; i++) {
    if (a->a[i] != b->a[i]) {
      return 0;
    }
  }

  return 1;
}

int main() {
  Decimal a = {{7, 4, 1}, 2}, b = {{7, 4, 1}, 2}, z = {{3, 5}, 1};

  if (is_equal(&a, &b))
    printf("a==b\n");

  if (!is_equal(&a, &z))
    printf("a!=z\n");

  printf("\n");

  return 0;
}
