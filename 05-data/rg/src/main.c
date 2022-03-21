#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef int (*generator_t)(int);

int gen_test(int x) {
  return (3 * x + 2) % 5 + 4;
}

unsigned cycle_len(generator_t gen) {
  int      slow = 0, fast = 0;
  unsigned result = 0;

  slow = fast = gen(slow);

  for (;;) {
    fast = gen(gen(fast));
    slow = gen(slow);

    if (slow == fast) {
      break;
    }
  }

  slow = gen(0);

  while (slow != fast) {
    slow = gen(slow);
    fast = gen(fast);
  }

  for (;;) {
    slow   = gen(slow);
    result = result + 1;

    if (slow == fast) {
      break;
    }
  }

  return result;
}

int main() {
  printf("%d\n", cycle_len(gen_test));
}
