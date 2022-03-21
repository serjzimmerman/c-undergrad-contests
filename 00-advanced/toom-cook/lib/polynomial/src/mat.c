#include "mat.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct matn_t *matn_init(size_t n, size_t m) {
  struct matn_t *mat = calloc(1, sizeof(struct matn_t) + n * sizeof(struct frac_t *));
  mat->n             = n;
  mat->m             = m;

  if (!mat) {
    return NULL;
  }

  mat->array = calloc(n * m, sizeof(struct frac_t));

  if (!mat->array) {
    free(mat);
    return NULL;
  }

  for (size_t i = 0; i < n * m; i++) {
    mat->array[i] = frac_from_num(0);
  }

  for (size_t i = 0; i < n; i++) {
    mat->rows[i] = mat->array + i * m;
  }

  return mat;
}

struct matn_t *matn_init_from_array(struct frac_t *array, size_t n, size_t m) {
  struct matn_t *mat = matn_init(n, m);

  if (!mat) {
    return NULL;
  }

  memcpy(mat->array, array, mat->n * mat->m * sizeof(struct frac_t));

  return mat;
}

struct matn_t *matn_init_from_num_array(int *array, size_t n, size_t m) {
  struct matn_t *mat = matn_init(n, m);

  if (!mat) {
    return NULL;
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      (mat->rows[i])[j] = frac_from_num(array[i * n + j]);
    }
  }

  return mat;
}

void matn_free(struct matn_t *mat) {
  assert(mat);
  free(mat->array);
  free(mat);
}

struct matn_t *matn_unity(size_t n) {
  struct matn_t *unity = matn_init(n, n);

  if (!unity) {
    return NULL;
  }

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      (unity->rows[i])[j] = (i == j ? frac_from_num(1) : frac_from_num(0));
    }
  }

  return unity;
}

void mat_util_2array_c_sum(struct frac_t *array1, struct frac_t *array2, struct frac_t c, size_t n) {
  assert(array1);
  assert(array2);

  for (size_t i = 0; i < n; i++) {
    array1[i] = frac_add(array1[i], frac_mult_frac(array2[i], c));
  }
}

void mat_util_1array_c(struct frac_t *array1, struct frac_t c, size_t n) {
  assert(array1);

  for (size_t i = 0; i < n; i++) {
    array1[i] = frac_mult_frac(array1[i], c);
  }
}

size_t mat_find_leader_n(struct matn_t *mat, size_t n, int *z) {
  size_t i, j;
  assert(mat);

  int leader = n;
  int zeros  = 0;

  for (j = 0; j < mat->m && frac_iszero(mat->rows[n][j]); j++) {
    zeros++;
  }

  for (i = n; i < mat->n; i++) {
    for (j = 0; frac_iszero(mat->rows[i][j]) && j < mat->m; j++) {
    }

    if (j < zeros) {
      leader = i;
      zeros  = j;
    }
  }

  if (zeros == mat->m) {
    *z = -1;
  } else {
    *z = zeros;
  }

  return leader;
}

void mat_util_swap_array_pointers(struct frac_t **array1, struct frac_t **array2) {
  struct frac_t *temp = *(array1);
  *(array1)           = *(array2);
  *(array2)           = temp;
}

struct matn_t *matn_convert_to_row_echelon(struct matn_t *mat) {
  size_t leader;
  int    z;

  assert(mat);

  for (size_t i = 0; i < mat->n; i++) {
    if (i >= mat->m) {
      break;
    }

    leader = mat_find_leader_n(mat, i, &z);

    if (z == -1) {
      break;
    }

    if (leader != i) {
      mat_util_swap_array_pointers(&mat->rows[leader], &mat->rows[i]);
    }

    for (size_t j = 0; j < mat->n; j++) {
      if (j == i) {
        continue;
      }
      mat_util_2array_c_sum(mat->rows[j], mat->rows[i], frac_negative(frac_div_frac(mat->rows[j][z], mat->rows[i][z])),
                            mat->m);
    }
  }

  return mat;
}

struct matn_t *matn_find_inverse(struct matn_t *mat) {
  struct frac_t c;
  int           z;

  assert(mat);
  assert(mat->n == mat->m);

  if (mat->n != mat->m) {
    return NULL;
  }

  struct matn_t *inv = matn_unity(mat->n);

  for (size_t i = 0; i < mat->n; i++) {
    if (i >= mat->m) {
      break;
    }

    size_t leader = mat_find_leader_n(mat, i, &z);
    if (z == -1) {
      break;
    }

    if (leader != i) {
      mat_util_swap_array_pointers(&mat->rows[leader], &mat->rows[i]);
      mat_util_swap_array_pointers(&inv->rows[leader], &inv->rows[i]);
    }

    for (size_t j = 0; j < mat->n; j++) {
      if (j == i) {
        continue;
      }

      c = frac_inverse(mat->rows[i][z]);
      mat_util_1array_c(mat->rows[i], c, mat->m);
      mat_util_1array_c(inv->rows[i], c, mat->m);

      c = frac_negative(mat->rows[j][z]);
      mat_util_2array_c_sum(mat->rows[j], mat->rows[i], c, mat->m);
      mat_util_2array_c_sum(inv->rows[j], inv->rows[i], c, mat->m);
    }
  }

  return inv;
}