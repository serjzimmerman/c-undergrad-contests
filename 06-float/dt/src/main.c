#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct matn_t {
  float *array;
  size_t n, m;
  float *rows[];
};

struct matn_t *matn_init(size_t n, size_t m) {
  struct matn_t *mat = calloc(1, sizeof(struct matn_t) + n * sizeof(float *));
  mat->n             = n;
  mat->m             = m;

  if (!mat) {
    return NULL;
  }

  mat->array = calloc(n * m, sizeof(float));

  if (!mat->array) {
    free(mat);
    return NULL;
  }

  memset(mat->array, 0, n * m);

  for (size_t i = 0; i < n; i++) {
    mat->rows[i] = mat->array + i * m;
  }

  return mat;
}

struct matn_t *matn_init_from_array(float *array, size_t n, size_t m) {
  struct matn_t *mat = matn_init(n, m);

  if (!mat) {
    return NULL;
  }

  memcpy(mat->array, array, mat->n * mat->m * sizeof(float));

  return mat;
}

void mat_util_swap_array_pointers(float **array1, float **array2) {
  float *temp = *(array1);
  *(array1)   = *(array2);
  *(array2)   = temp;
}

size_t mat_find_leader_n(struct matn_t *mat, size_t n, int *z) {
  size_t i, j, leader;
  int    zeros = 0;

  assert(mat);

  leader = n;
  for (j = 0; j < mat->m && !mat->rows[n][j]; j++) {
    zeros++;
  }

  for (i = n; i < mat->n; i++) {
    for (j = 0; !mat->rows[i][j] && j < mat->m; j++) {
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

void mat_util_2array_c_sum(float *array1, float *array2, float c, size_t n) {
  assert(array1);
  assert(array2);

  for (size_t i = 0; i < n; i++) {
    array1[i] += array2[i] * c;
  }
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
      mat_util_2array_c_sum(mat->rows[j], mat->rows[i], (-1) * mat->rows[j][z] / mat->rows[i][z], mat->m);
      mat->rows[j][z] = 0.f;
    }
  }

  return mat;
}

float matn_mul_diag(struct matn_t *mat) {
  int   i;
  float r = 1;

  assert(mat->n == mat->m);

  for (i = 0; i < mat->n; i++) {
    r *= mat->rows[i][i];
  }

  return r;
}

void matn_print(struct matn_t *mat) {
  for (size_t i = 0; i < mat->n; i++) {
    for (size_t j = 0; j < mat->m; j++) {
      printf((j == mat->m - 1 ? "%f\n" : "%f "), mat->rows[i][j]);
    }
  }
}

int main() {
  int            res, i;
  unsigned       n;
  struct matn_t *mat;
  float          det;

  res = scanf("%u", &n);

  if (res != 1 || !n) {
    fprintf(stderr, "Invalid input, exiting\n");
    exit(EXIT_FAILURE);
  }

  mat = matn_init(n, n);

  for (i = 0; i < n * n; i++) {
    scanf("%f", &mat->array[i]);
  }

  mat = matn_convert_to_row_echelon(mat);
  det = matn_mul_diag(mat);

  printf("%.0f\n", det);

  matn_print(mat);
}
