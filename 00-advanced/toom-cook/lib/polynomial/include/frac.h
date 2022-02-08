#ifndef FRAC_H
#define FRAC_H

struct frac_t {
  int numerator, denominator;
};

struct frac_t frac_add(struct frac_t a, struct frac_t b);
struct frac_t frac_mult_frac(struct frac_t a, struct frac_t b);
struct frac_t frac_mult_num(struct frac_t a, int n);
struct frac_t frac_inverse(struct frac_t a);
struct frac_t frac_negative(struct frac_t a);
struct frac_t frac_from_num(int n);
int frac_iszero(struct frac_t a);
struct frac_t frac_init(int numerator, int denominator);
struct frac_t frac_div_frac(struct frac_t a, struct frac_t b);

#endif