#include "frac.h"
#include <assert.h>

int iabs(int x) {
  return (x < 0) ? -x : x;
}

int isign(int x) {
  return (x < 0) ? -1 : 1;
}

int eu_mod(int x, int y) {
  assert(y != 0);

  int r = x % y;

  if (r < 0) {
    r += iabs(y);
  }

  return r;
}

int gcd(int x, int y) {
  int q;

  assert(y != 0);

  while ((q = eu_mod(x, y)) != 0) {
    x = y;
    y = q;
  }

  return y;
}

struct frac_t frac_normalize(struct frac_t a) {
  assert(a.denominator);

  int g = gcd(iabs(a.numerator), a.denominator);

  if (a.numerator == 0) {
    a.denominator = 1;
  } else {
    a.numerator = a.numerator / g;
    a.denominator = a.denominator / g;
  }

  return a;
}

struct frac_t frac_add(struct frac_t a, struct frac_t b) {
  struct frac_t r;

  assert(a.denominator);
  assert(b.denominator);

  int g = gcd(a.denominator, b.denominator);
  r.denominator = a.denominator * b.denominator;
  r.numerator = a.numerator * b.denominator + b.numerator * a.denominator;

  return frac_normalize(r);
}

struct frac_t frac_mult_frac(struct frac_t a, struct frac_t b) {
  struct frac_t r;

  assert(a.denominator);
  assert(b.denominator);

  if (a.numerator == 0 || b.numerator == 0) {
    r.numerator = 0;
    r.denominator = 1;
    return r;
  }

  r.numerator = a.numerator * b.numerator;
  r.denominator = a.denominator * b.denominator;
  int g = gcd(iabs(r.numerator), r.denominator);

  if (r.numerator == 0) {
    r.denominator = 1;
  } else {
    r.numerator = r.numerator / g;
    r.denominator = r.denominator / g;
  }

  return r;
}

struct frac_t frac_div_frac(struct frac_t a, struct frac_t b) {
  struct frac_t t;

  assert(a.denominator);
  assert(b.denominator);

  t = frac_inverse(b);

  return frac_mult_frac(a, t);
}

struct frac_t frac_mult_num(struct frac_t a, int n) {
  struct frac_t t;

  assert(a.denominator);

  t.numerator = n;
  t.denominator = 1;

  return frac_mult_frac(a, t);
}

struct frac_t frac_inverse(struct frac_t a) {
  struct frac_t r;

  assert(a.denominator);

  r.numerator = a.denominator * isign(a.numerator);
  r.denominator = iabs(a.numerator);

  return r;
}

struct frac_t frac_negative(struct frac_t a) {
  assert(a.denominator);

  a.numerator = (-1) * a.numerator;

  return a;
}

struct frac_t frac_from_num(int n) {
  struct frac_t r;

  r.denominator = 1;
  r.numerator = n;

  return r;
}

int frac_iszero(struct frac_t a) {
  assert(a.denominator);

  return (!a.numerator);
}

struct frac_t frac_init(int numerator, int denominator) {
  struct frac_t r;

  assert(denominator);

  r.numerator = numerator;
  r.denominator = denominator;

  return r;
}