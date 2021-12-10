#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strbin(char *dst, unsigned int x) {
  int i;

  memset(dst, 0, 32);

  for (i = 31; i > 0;) {
    if (x & (1 << i)) {
      break;
    }
    i--;
  }

  if (i == 0) {
    dst[0] = '0';
    return dst;
  }

  for (; i > 0; i--) {
    if (x & (1 << i)) {
      strcat(dst, "1");
    } else {
      strcat(dst, "0");
    }
  }

  return dst;
}

int main() {
  char dst[128];

  printf("%s\n", strbin(dst, UINT_MAX));
  return 0;
}
