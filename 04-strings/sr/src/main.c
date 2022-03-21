#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void swap_chars(char *x, char *y) {
  char temp = *x;
  *x        = *y;
  *y        = temp;
}

void revstrn(char *str, int n) {
  int low = 0, high = n - 1;

  while (low <= high) {
    swap_chars(&str[high--], &str[low++]);
  }
}

void readnc(FILE *stream, char *str, const int n) {
  char c;
  int  i = 0;

  while (isspace(c = getc(stream))) {
    ;
  }
  while (i < n) {
    str[i++] = c;
    c        = getc(stream);
  }
  str[i] = '\0';
}

void reverse_substrings(char *str, const char *substr, const int n) {
  char *traveller;
  int   stringlen = strlen(str);

  traveller = str;

  while ((traveller = strstr(traveller, substr)) != NULL && traveller - str < stringlen) {
    revstrn(traveller, n);
    traveller = traveller + n;
  }
}

int main() {
  int   sublen = 0, stringlen = 0;
  char *string, *substring;

  scanf("%d", &sublen);
  substring = calloc(1, sizeof(char) * (sublen + 1));
  readnc(stdin, substring, sublen);

  scanf("%d", &stringlen);
  string = calloc(1, sizeof(char) * (stringlen + 1));
  readnc(stdin, string, stringlen);

  reverse_substrings(string, substring, sublen);
  printf("%s\n", string);
}
