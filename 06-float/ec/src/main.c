#include <assert.h>
#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS 1e-5

float compute_function(float a, float b, float c, float x) {
  return a * powf(x, 2) * sinf(x) + b * x * cosf(x) + c;
}

float signf(float x) {
  return (signbit(x) ? -1.f : 1.f);
}

float solve_equation_riddres(float a, float b, float c, float n) {
  float left, right, mid, lval, rval, mval, mnew, newval;

  left  = -n;
  right = n;

  lval = compute_function(a, b, c, left);
  rval = compute_function(a, b, c, right);

  while (signbit(lval * rval)) {
    assert(signbit(lval) != signbit(rval));

    mid  = (left + right) / 2.f;
    mval = compute_function(a, b, c, mid);

    mnew   = mid + (mid - left) * signf(lval - rval) * mval / sqrtf(powf(mval, 2) - lval * rval);
    newval = compute_function(a, b, c, mnew);

    if (newval < EPS) {
      return mnew;
    }

    if (signbit(newval)) {
      right = mnew;
    } else {
      left = mnew;
    }
  }

  return 0;
}

int main() {
  float a, b, c, n, x;
  int   res;

  res = scanf("%f %f %f %f", &a, &b, &c, &n);

  if (res != 4) {
    exit(EXIT_FAILURE);
  }

  x = solve_equation_riddres(a, b, c, n);
  if (!x) {
    printf("0.0\n");
  } else {
    printf("%.5f", x);
  }
}
