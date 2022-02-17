#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frac.h"
#include "mat.h"
#include "poly.h"

struct matn_t *interpolation_matrix_calculated[16];
#define MIN(x, y) ((x) < (y) ? (x) : (y))

struct poly_t *poly_init(size_t n, int *array) {
  struct poly_t *poly;

  poly = calloc(1, sizeof(struct poly_t));

  if (!poly) {
    return NULL;
  }

  poly->n = n;
  if (array) {
    poly->array = array;
    return poly;
  }

  poly->array = calloc(n, sizeof(int));
  if (!poly->array) {
    free(poly);
    return NULL;
  }

  return poly;
}

void poly_free(struct poly_t *poly, int freedata) {
  if (freedata) {
    free(poly->array);
  }
  free(poly);
}

struct poly_t *poly_add(struct poly_t *a, struct poly_t *b) {
  size_t n = MIN(a->n, b->n);

  for (size_t i = 0; i < n; i++) {
    a->array[i] = a->array[i] + b->array[i];
  }

  return a;
}

struct poly_t *poly_mult_naive(struct poly_t *a, struct poly_t *b) {
  struct poly_t *poly = poly_init(a->n + b->n - 1, NULL);

  if (!poly) {
    return NULL;
  }

  for (size_t i = 0; i < a->n; i++) {
    for (size_t j = 0; j < b->n; j++) {
      poly->array[i + j] += a->array[i] * b->array[j];
    }
  }

  return poly;
}

void poly_split(struct poly_t *poly, struct poly_t *array, size_t k) {
  size_t q, r, j;

  /* Divide polynomial into roughly equal parts */
  q = poly->n / k;
  r = poly->n % k;

  j = 0;

  for (size_t i = 0; i < k; i++) {
    (array[i]).array = &poly->array[j];
    if (r) {
      (array[i]).n = q + 1;
      r--;
    } else {
      (array[i]).n = q;
    }
    j += (array[i]).n;
  }
}

void toom_cook_matrix_row(struct frac_t *array, int y, size_t m) {
  int b = 1;

  for (size_t i = 0; i < m; i++) {
    array[i] = frac_from_num(b);
    b *= y;
  }
}

struct matn_t *toom_cook_general_matrix(size_t n, size_t m) {
  struct matn_t *mat;

  mat = matn_init(n, m);

  assert(n > 1);
  if (n < 2) {
    return NULL;
  }

  int y = 1;
  for (size_t i = 1; i < mat->n - 1; i++) {
    toom_cook_matrix_row(mat->rows[i], y, mat->m);
    if (i & 0x1) {
      y *= (-1);
    } else {
      y = (-1) * y + 1;
    }
  }

  mat->rows[0][0] = frac_from_num(1);
  /* Set lats row to 0 0 0 0 ... 1, which corresponds to y = inf */
  mat->rows[mat->n - 1][mat->m - 1] = frac_from_num(1);

  return mat;
}

struct matn_t *toom_cook_interpolation_matrix(size_t k) {
  struct matn_t *mat, *inv;

  mat = toom_cook_general_matrix(2 * k - 1, 2 * k - 1);
  inv = matn_find_inverse(mat);
  matn_free(mat);

  return inv;
}

struct poly_t *poly_mult_toom_cook(struct poly_t *a, struct poly_t *b, size_t k) {
  struct poly_t splita[k], splitb[k];
  struct poly_t *arraya[2 * k - 1], *arrayb[2 * k - 1], *arrayc[2 * k - 1];
  struct poly_t **cc;
  struct matn_t *mat;

  if (a->n == 1 || b->n == 1) {
    return
  }

  mat = toom_cook_general_matrix(2 * k - 1, k);

  poly_split(a, splita, k);
  poly_split(b, splitb, k);

  for (size_t i = 0; i < 2 * k - 1; i++) {
    arraya[i] = poly_init(splita[0].n, NULL);
    arrayb[i] = poly_init(splitb[0].n, NULL);
  }

  for (size_t i = 0; i < 2 * k - 1; i++) {
    for (size_t j = 0; j < k; j++) {
      for (size_t t = 0; t < splita[j].n; t++) {
        arraya[i]->array[t] += splita[j].array[t] * mat->rows[i][j].numerator / mat->rows[i][j].denominator;
      }
    }
  }

  for (size_t i = 0; i < 2 * k - 1; i++) {
    for (size_t j = 0; j < k; j++) {
      for (size_t t = 0; t < splitb[j].n; t++) {
        arrayb[i]->array[t] += splitb[j].array[t] * mat->rows[i][j].numerator / mat->rows[i][j].denominator;
      }
    }
  }

  for (size_t i = 0; i < 2 * k - 1; i++) {
    arrayc[i] = poly_mult_toom_cook(arraya[i], arrayb[i], k);
  }

  cc = calloc(2 * k - 1, sizeof(struct poly_t *));

  for (size_t i = 0; i < 2 * k - 1; i++) {
    cc[i] = poly_init()
  }
}