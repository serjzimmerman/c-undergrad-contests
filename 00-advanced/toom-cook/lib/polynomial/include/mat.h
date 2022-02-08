#ifndef MAT_H
#define MAT_H

#include "frac.h"
#include <ctype.h>
#include <stdlib.h>

struct matn_t {
  struct frac_t *array;
  size_t n, m;
  struct frac_t *rows[];
};

struct matn_t *matn_init(size_t n, size_t m);
void matn_free(struct matn_t *mat);
struct matn_t *matn_unity(size_t n);
struct matn_t *matn_init_from_array(struct frac_t *array, size_t n, size_t m);
struct matn_t *matn_init_from_num_array(int *array, size_t n, size_t m);

struct matn_t *matn_convert_to_row_echelon(struct matn_t *mat);
struct matn_t *matn_find_inverse(struct matn_t *mat);

#endif