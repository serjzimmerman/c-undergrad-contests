#ifndef POLY_H
#define POLY_H

#include <ctype.h>
#include <stdio.h>

struct poly_t {
  int *array;
  size_t n;
};

struct poly_t *poly_init(size_t n, int *array);
void poly_free(struct poly_t *poly, int freedata);
struct poly_t *poly_add(struct poly_t *a, struct poly_t *b);
struct poly_t *poly_mult_naive(struct poly_t *a, struct poly_t *b);
void poly_split(struct poly_t *poly, struct poly_t *array, size_t n);
struct matn_t *toom_cook_interpolation_matrix(size_t k);
struct poly_t *poly_mult_toom_cook(struct poly_t *a, struct poly_t *b, size_t k);

#endif