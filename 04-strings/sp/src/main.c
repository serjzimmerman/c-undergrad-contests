#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int substrcnt(char *str, const char *substr) {
  int substrlen, cnt = 0;

  substrlen = strlen(substr);

  while ((str = strstr(str, substr)) != NULL) {
    cnt++;
    str += substrlen;
  }

  return cnt;
}

char **substrnptr(char *str, const char *substr, const int cnt) {
  char **result;
  int    i = 0, sublen;

  sublen = strlen(substr);
  result = calloc(cnt, sizeof(char *));

  while ((str = strstr(str, substr)) != NULL) {
    result[i++] = str;
    str += sublen;
  }

  return result;
}

char *replace(char *str, char const *from, char const *to) {
  char *result, **subptr;
  int   nsub, fromlen, tolen, i = 0, j = 0;

  tolen   = strlen(to);
  fromlen = strlen(from);
  nsub    = substrcnt(str, from);

  if ((result = calloc(1 + strlen(str) + nsub * (tolen - fromlen), sizeof(char))) == NULL) {
    return NULL;
  }

  subptr = substrnptr(str, from, nsub);

  while (*str != '\0') {
    if (i < nsub && str == subptr[i]) {
      memcpy(&result[j], to, tolen * sizeof(char));
      str += fromlen;
      i++;
      j += tolen;
    } else {
      result[j++] = *str++;
    }
  }

  result[j] = '\0';

  free(subptr);

  return result;
}

int main() {
  char  str[] = "Hello, %username, how are you, %username", from[] = "%username", to[] = "Eric, the Blood Axe";
  char *result;

  printf("%s\n", (result = replace(str, from, to)));

  free(result);

  return 0;
}
