#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int cmp_int(const void *a, const void *b) {
  return *(int *)a - *(int *)b;
}

int coverpoints(int *pts, int n, int unitlen) {
  int i, c = 1, end;

  if (n == 0) {
    return 0;
  }

  qsort(pts, n, sizeof(int), cmp_int);

  end = pts[0] + unitlen;
  for (i = 0; i < n; i++) {
    if (pts[i] <= end) {
      continue;
    }
    end = pts[i] + unitlen;
    c++;
  }

  return c;
}

int main() {
  int arr[] = {2, 3, 4, 1, 6, 5};

  printf("%d", coverpoints(arr, sizeof(arr) / sizeof(int), 1));

  return 0;
}