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

float solve_equation(float a, float b, float c, float n) {
  float left, right, mid, lval, rval, mval;

  left = -n;
  right = n;

  lval = compute_function(a, b, c, left);
  rval = compute_function(a, b, c, right);

  while (lval * rval < 0) {
    mid = (left + right) / 2;
    mval = compute_function(a, b, c, mid);

    if (fabsf(mval) < EPS) {
      return mid;
    }

    if (mval * lval < 0) {
      right = mid;
    } else if (mval * rval < 0) {
      left = mid;
    }
  }

  return 0;
}

int main() {
  float a, b, c, n, x;
  int res;

  res = scanf("%f %f %f %f", &a, &b, &c, &n);

  if (res != 4) {
    exit(EXIT_FAILURE);
  }

  x = solve_equation(a, b, c, n);
  if (!x) {
    printf("0.0\n");
  } else {
    printf("%.5f", x);
  }
}
