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

/* ----------------------------------------------------------------
    Sequence s_n: a -> baa -> cbaabaa, find k-th symbol in n-th string Length is determined with the following recursive
    equation: len(a_{n+1}) = 2 * len(a_{n}) - 1 If k == 1, then the answer is immediately known, and is equal to n-th
    alphabetic char, otherwise divide and conquer
   ---------------------------------------------------------------- */

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
