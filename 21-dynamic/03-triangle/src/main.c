/* https://kpm8.mipt.ru:44367/cgi-bin/new-register?contest_id=170104&locale_id=1
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(x, y) (((x) > (y)) ? (x) : (y))

int triange_get(int *tri, int row, int column) {
  return tri[row * (row + 1) / 2 + column];
}

void triangle_set(int *tri, int row, int column, int val) {
  tri[row * (row + 1) / 2 + column] = val;
}

int genarrmaxn(int *arr, int n) {
  int i, c;

  c = arr[0];

  for (i = 1; i < n; i++) {
    if (arr[i] > c) {
      c = arr[i];
    }
  }

  return c;
}

int *triangle_row(int *tri, int row) {
  return &tri[row * (row + 1) / 2];
}

int triangle_max(int *tri, int *buf, int n) {
  int i, j;
  /* i for iterating over rows, j - for columns */
  triangle_set(buf, 0, 0, triange_get(tri, 0, 0));

  for (i = 1; i < n; i++) {
    for (j = 0; j <= i; j++) {
      if (j == 0) {
        triangle_set(buf, i, j, triange_get(tri, i, j) + triange_get(buf, i - 1, j));
      } else if (j == i) {
        triangle_set(buf, i, j, triange_get(tri, i, j) + triange_get(buf, i - 1, j - 1));
      } else {
        triangle_set(buf, i, j,
                     triange_get(tri, i, j) + max(triange_get(buf, i - 1, j - 1), triange_get(buf, i - 1, j)));
      }
    }
  }

  return genarrmaxn(triangle_row(buf, n - 1), n);
}

void print_triangle(int *tri, int n) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j <= i; j++) {
      printf("%d,", triange_get(tri, i, j));
    }
    printf("\n");
  }
}

int main() {
  int i, n, s, *tri, *buf;

  scanf("%d", &n);

  s = n * (n + 1) / 2;

  tri = (int *)calloc(s, sizeof(int));
  buf = (int *)calloc(s, sizeof(int));

  for (i = 0; i < s; i++) {
    scanf("%d", &tri[i]);
  }

  printf("%d\n", triangle_max(tri, buf, n));

  free(buf);
  free(tri);

  return 0;
}
