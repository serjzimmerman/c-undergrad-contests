#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_NAF 128

typedef struct {
  char array[MAX_NAF];
} naf;

naf convert_to_naf(unsigned long long n) {
  int i = 0;
  naf r = {0};

  while (n > 0) {
    if (n & 1) {
      r.array[i] = 2 - n % 4;
      n -= r.array[i];
    } else {
      r.array[i] = 0;
    }
    n >>= 1;
    i++;
  }

  return r;
}

void print_naf(naf n) {
  int i, j;

  for (j = MAX_NAF - 1; n.array[j] == 0; j--) {
  }
  for (i = j; i >= 0; i--) {
    printf("%d ", n.array[i]);
  }

  printf("\n");
}

int main() {
  int res;
  naf r;
  unsigned long long x;

  res = scanf("%llu", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  r = convert_to_naf(x);
  print_naf(r);

  return 0;
}
