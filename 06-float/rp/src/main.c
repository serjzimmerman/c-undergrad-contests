#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned float_as_uint(float x) {
  unsigned u;

  memcpy(&u, &x, sizeof(unsigned));

  return u;
}

struct edges_t {
  float top, bottom;
};

struct edges_t get_edges(int n, int d) {
  struct edges_t result = {0};
  float fn, fd;

  fn = (float)n;
  fd = (float)d;

  fesetround(FE_DOWNWARD);
  result.bottom = fn / fd;
  fesetround(FE_UPWARD);
  result.top = fn / fd;

  return result;
}

struct expman_t {
  unsigned exp, man;
};

struct expman_t bin_from_float(float x) {
  struct expman_t result;
  unsigned temp;

  temp = float_as_uint(x);

  result.man = temp & 0x7fffff;
  result.exp = (temp >> 23) & 0xff;

  return result;
}

int main() {
  int x, y, res;
  struct edges_t result;
  struct expman_t first, second;

  res = scanf("%d %d", &x, &y);

  if (res != 2) {
    exit(EXIT_FAILURE);
  }

  result = get_edges(x, y);

  first = bin_from_float(result.bottom);
  second = bin_from_float(result.top);

  if (first.exp == second.exp && first.man == second.man) {
    printf("0x%x 0x%x", first.exp, first.man);
  } else {
    printf("0x%x 0x%x 0x%x 0x%x", first.exp, first.man, second.exp, second.man);
  }
}
