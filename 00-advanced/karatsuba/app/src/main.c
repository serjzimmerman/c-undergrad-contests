#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "polynomial.h"

int main() {
  int           i, n, m, *array1, *array2;
  polynomial_t *p1, *p2, *p3;

  scanf("%d %d", &n, &m);

  p1 = polynomial_init_malloc(n);
  p2 = polynomial_init_malloc(m);

  if (!(p1 && p2)) {
    abort();
  }

  for (i = 0; i < n; i++) {
    scanf("%d", &(p1->coefficients[i]));
  }
  for (i = 0; i < m; i++) {
    scanf("%d", &(p2->coefficients[i]));
  }

  if (!(p3 = polynomial_mul_karatsuba(p1, p2))) {
    abort();
  }

  polynomial_print(p3);

  polynomial_free(p1);
  polynomial_free(p2);
  polynomial_free(p3);

  return 0;
}