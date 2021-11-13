#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strcat_r(char *dest, const char *src, int *bufsz) {
  int i = 0, j = 0, dlen, slen;

  dlen = strlen(dest);
  slen = strlen(src);

  if (*bufsz < (dlen + slen + 1)) {
    *bufsz = dlen + slen + 1;
    dest = (char *)realloc(dest, sizeof(char) * (*bufsz));
  }

  while (dest[i] != '\0') {
    i++;
  }

  while (src[j] != '\0') {
    dest[i++] = src[j++];
  }
  dest[i] = '\0';

  return dest;
}

int main() {
  char *dest, *src;
  int size = 20;

  dest = calloc(20, sizeof(char));
  src = calloc(20, sizeof(char));

  strcpy(src, "Hello, World!");
  strcpy(dest, "1232212");

  dest = strcat_r(dest, src, &size);

  printf("string: %s, new size: %d\n", dest, size);

  free(dest);
  free(src);

  return 0;
}
