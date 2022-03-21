#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_string_sum(char *string) {
  int sum = 0;

  while (*string != '\0') {
    sum += *string++;
  }

  return sum;
}

int main() {
  int  sum = 0;
  char c;

  while ((c = getc(stdin)) != EOF) {
    sum += c;
  }

  printf("%d", sum);
}
