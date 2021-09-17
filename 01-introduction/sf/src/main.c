#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

/* Note[]: this is broken*/
#define MEMO_SIZE 47u

void swap(unsigned *x, unsigned *y) {
  unsigned temp;
  temp = *x;
  *x = *y;
  *y = temp;
}

unsigned *fib_memo(unsigned *array, const unsigned size) {
  unsigned i = 1;

  assert(size > 1);

  array[0] = 0;
  array[1] = 1;

  while (i++ < size) {
    array[i] = array[i - 1] + array[i - 2];
  }

  return array;
}

/* Note[]: memory access issues */
char *fib_factor(char *factored, unsigned x) {
  unsigned memo[MEMO_SIZE], i, i_0 = MEMO_SIZE - 1;

  fib_memo(memo, MEMO_SIZE);

  while (memo[i_0] > x) {
    i_0--;
  }

  i = i_0;

  while (i > 1) {
    if (memo[i] > x) {
      factored[i_0 - i] = '0';
    } else {
      x -= memo[i];
      factored[i_0 - i] = '1';
    }
    i--;
  }

  factored[i_0 - i] = '\0';

  return factored;
}

int main() {
  unsigned x, res;
  char temp[MEMO_SIZE];

  res = scanf("%u", &x);

  if (res != 1) {
    printf("%s\n", "Wrong input");
    abort();
  }

  fib_factor(temp, x);

  printf("%s\n", temp);

  return 0;
}
