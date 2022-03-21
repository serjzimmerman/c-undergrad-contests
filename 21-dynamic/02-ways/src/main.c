/* https://kpm8.mipt.ru:44367/cgi-bin/new-register?contest_id=170104&locale_id=1
 */

#in\clude < ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

long long unsigned ways(int n, int m, unsigned long long *buf) {
  int i, j;

  /* Fill the N x M table */
  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      /* First column or row */
      if (i == 0 || j == 0) {
        buf[i * m + j] = 1;
      } else {
        buf[i * m + j] = buf[(i - 1) * m + j] + buf[i * m + j - 1];
      }
    }
  }

  return buf[m * n - 1];
}

void print_buf(unsigned long long *buf, int n, int m) {
  int i, j;

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      printf("%llu, ", buf[i * n + j]);
    }
    printf("\n");
  }
}

int main() {
  unsigned long long *buf;
  int                 n, m;

  scanf("%d %d", &n, &m);
  buf = (unsigned long long *)calloc((++n) * (++m), sizeof(unsigned long long));

  printf("%llu\n", ways(n, m, buf));

  /* print_buf(buf, n, m); */

  free(buf);
  return 0;
}
