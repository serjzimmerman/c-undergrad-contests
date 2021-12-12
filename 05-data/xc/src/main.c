#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_MAX 15

typedef int (*get_hash_t)(const char *s);

int simple_hash(const char *s) {
  unsigned res;

  while (*s != '\0') {
    res += *(s++);
  }

  return res % (HASH_MAX - 1);
}

int ncollisions(char **strs, int n, get_hash_t f) {
  unsigned narray[HASH_MAX + 1] = {0}, i, collisions = 0;

  for (i = 0; i < n; i++) {
    narray[f(strs[i])] += 1;
  }

  for (i = 0; i < HASH_MAX; i++) {
    if (narray[i] != 0) {
      collisions += narray[i] - 1;
    }
  }

  return collisions;
}

int main() {
  char *strings[] = {"ad", "bc"};

  printf("%d\n", ncollisions(&strings[0], 2, simple_hash));
}
