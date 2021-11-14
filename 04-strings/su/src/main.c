#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *strstrci(char const *needle, char const *haystack) {
  int i = 0, j = 0;

  while (haystack[i] != '\0') {
    for (;;) {
      if (haystack[i + j] == '\0') {
        return NULL;
      }
      if (needle[j] == '\0') {
        return haystack + i;
      }
      if (tolower(haystack[i + j]) == tolower(needle[j])) {
        j++;
      } else {
        break;
      }
    }
    j = 0;
    i++;
  }

  return NULL;
}

int main() {
  printf("%s", strstrci("Abc", "abcstr"));
  return 0;
}
