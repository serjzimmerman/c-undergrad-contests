#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Solution_Pp {
  unsigned long pp, mod;
};

void swap(unsigned long *x, unsigned long *y) {
  unsigned long temp;

  temp = *x;
  *x   = *y;
  *y   = temp;
}

unsigned long get_pp(unsigned long n) {
  unsigned long i, r, r_old, r_old_old, temp;

  r_old_old = 0;
  r_old     = 1;
  r         = 1;
  i         = 1;

  for (;; i++) {
    temp = r;
    r    = (r_old_old + r_old) % n;
    swap(&temp, &r_old);
    r_old_old = r;

    if (r_old == 0 && r == 1) {
      return i;
    }
  }
}

void fib_modulo(struct Solution_Pp *sol, unsigned long x, unsigned long n) {
  unsigned long i, r, r_old, r_old_old, temp;

  sol->pp = get_pp(n);

  x = x % sol->pp;

  if (x < 2) {
    sol->mod = x;
    return;
  }

  r_old_old = 0;
  r_old     = 1;
  r         = 1;
  i         = 1;

  while (i++ < x) {
    temp = r;
    r    = (r_old_old + r_old) % n;
    swap(&temp, &r_old);
    r_old_old = r;
  }

  sol->mod = r;
}

int main() {
  unsigned long      x, y, res;
  struct Solution_Pp sol;

  res = scanf("%lu%lu", &x, &y);

  if (res != 2 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

  fib_modulo(&sol, x, y);

  printf("%lu %lu \n", sol.mod, sol.pp);

  return 0;
}
