#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100

struct _Decimal {
  char a[N];      // number is a[0]*10^0 + a[1]*10^1 + ..+ a[n]*10^n
  unsigned int n; // наибольшая степень десяти
};
typedef struct _Decimal Decimal;

Decimal zero = {{0}, 0}; // представление 0 в виде структуры

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
  Decimal a = {{7, 4, 1}, 2}; // set number 147
  Decimal b = {{7, 4, 1}, 2}; // set number 147
  Decimal z = {{3, 5}, 1};    // set number 53

  if (is_equal(&a, &b))
    printf("a==b\n");

  if (!is_equal(&a, &z))
    printf("a!=z\n");

  printf("\n");

  return 0;
}
