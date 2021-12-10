#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char *const lookup_table[] = {"000", "001", "010", "011", "100", "101", "110", "111"};

int char2int(const char c) {
  return (c - '0');
}

char *convert(char *dst, const char *src) {
  int i = 0, j = 0, l = 0;

  if (strcmp(src, "0") == 0) {
    dst[0] = '0';
    dst[1] = '\0';

    return dst;
  }

  l = strlen(src);

  for (i = 0; i < l; i++) {
    memcpy(&dst[i * 3], lookup_table[char2int(src[i])], 3);
  }

  for (j = 0; dst[j] == '0'; j++) {
  }

  for (i = 0; i + j < l * 3; i++) {
    dst[i] = dst[i + j];
  }

  dst[i] = '\0';

  return dst;
}

int main() {
  char d[100];

  printf("%s\n", convert(d, "540"));

  return 0;
}
