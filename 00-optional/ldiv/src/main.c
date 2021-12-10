#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 128

struct _Decimal {
  /* a[0]*10^0 + a[1]*10^1 + ..+ a[n]*10^n */
  char a[N];
  unsigned int n;
};
typedef struct _Decimal Decimal;

Decimal zero = {{0}, 0};

void div10(Decimal *res, const Decimal *a) {
  if (a->n == 0) {
    memcpy(res, &zero, sizeof(Decimal));
    return;
  }

  memcpy(res->a, &a->a[1], a->n * sizeof(char));
  res->n = a->n - 1;
}

void print(Decimal *a) {
  int i;

  for (i = a->n; i >= 0; i--) {
    printf("%d", a->a[i]);
  }

  printf("\n");
}

int main() {
  Decimal a = {{9, 9, 9}, 2};
  Decimal res;

  div10(&res, &a);

  print(&res);

  return 0;
}
