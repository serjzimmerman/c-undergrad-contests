#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int ipow(int x, int p) {
  int r = 1;

  while (p--) {
    r *= x;
  }

  return r;
}

char sequence(int n, int k) {
  int len;

  if (k == 1) {
    return ('a' + (n - 1));
  } else {
    len = ipow(2, n - 1) - 1;
    if ((k - 1) <= len) {
      return sequence(n - 1, k - 1);
    } else {
      return sequence(n - 1, k - 1 - len);
    }
  }
}

int main() {
  int n, k;

  scanf("%d %d", &n, &k);

  printf("%c\n", sequence(n, k));
}
