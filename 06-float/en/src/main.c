#include <ctype.h>
#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EPS_VAL 1e-5
#define EPS_DER 1e-10

struct func_s {
  double function, deriv;
};

typedef struct func_s func_t;

func_t compute_function(double a, double b, double c, double x) {
  func_t result = {0};

  result.function = a * pow(x, 2) * sin(x) + b * x * cos(x) + c;
  result.deriv = cos(x) * (a * pow(x, 2) + b) + x * (2 * a - b) * sin(x);

  return result;
}

#define ITER_MAX 4096

double gradient_solve(double a, double b, double c, double x0) {
  int iterations = 0;
  func_t val;

  do {
    val = compute_function(a, b, c, x0);

    if (fabs(val.function) < EPS_VAL) {
      return x0;
    }

    iterations++;
    x0 = x0 - val.function / val.deriv;
  } while (fabs(val.deriv) > EPS_DER && iterations < ITER_MAX);

  return 0.f;
}

int main() {
  double a, b, c, x0, ans;
  int res;

  res = scanf("%lf %lf %lf %lf", &a, &b, &c, &x0);
  if (res != 4) {
    fprintf(stderr, "Invalid input");
    exit(EXIT_FAILURE);
  }

  ans = gradient_solve(a, b, c, x0);
  printf((ans == 0.f ? "%.1lf" : "%.5lf"), ans);
}
