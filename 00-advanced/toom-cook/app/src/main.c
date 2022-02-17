#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "frac.h"
#include "mat.h"
#include "poly.h"

#undef NDEBUG

#include <assert.h>

void matn_print(struct matn_t *mat) {
  for (size_t i = 0; i < mat->n; i++) {
    for (size_t j = 0; j < mat->m; j++) {
      printf((j == mat->m - 1 ? "%d / %d\n\n" : "%d / %d\t"), (mat->rows[i])[j].numerator,
             (mat->rows[i])[j].denominator);
    }
  }
}

int main() {
  int array1[3] = {1, 2, 1}, array2[3] = {1, 2, 1};

  struct matn_t *mat = matn_init_from_num_array(&array[0][0], 3, 3);
  struct matn_t *inv = matn_find_inverse(mat);

  matn_print(inv);
  matn_free(mat);
}