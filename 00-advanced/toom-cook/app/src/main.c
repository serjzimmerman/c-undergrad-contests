#include "frac.h"
#include "mat.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void matn_print(struct matn_t *mat) {
  for (size_t i = 0; i < mat->n; i++) {
    for (size_t j = 0; j < mat->m; j++) {
      printf((j == mat->m - 1 ? "%d / %d\n\n" : "%d / %d\t"), (mat->rows[i])[j].numerator,
             (mat->rows[i])[j].denominator);
    }
  }
}

int main() {
  int array[3][3] = {{2, -1, 0}, {0, 2, -1}, {-1, -1, 1}};

  struct matn_t *mat = matn_init_from_num_array(&array[0][0], 3, 3);
  matn_convert_to_row_echelon(mat);

  matn_print(mat);
  matn_free(mat);
}