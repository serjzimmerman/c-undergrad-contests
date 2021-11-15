#include <math.h>
#include <stdio.h>
#include <stdlib.h>

long int pow_int(long int n, long int power) {
  long int r = 1;

  while (power) {
    r *= n;
    power--;
  }

  return r;
}

long int digit_num(long int n) {
  long int digits = 0;

  while (n >= pow_int(10, 1 + digits++)) {
    ;
  }

  return digits;
}

long int digit_sum(long int n) {
  long int digits = digit_num(n);
  int i;

  if (digits > 1) {
    long int temp = 0;

    for (i = 1; i < digits + 1; i++) {
      temp = temp + n % 10;
      n = n / 10;
    }

    return digit_sum(temp);
  }

  else {

    return n;
  }
}

int main() {
  long int temp = 0;

  scanf("%ld", &temp);
  printf("%ld\n", digit_sum(temp));

  return 0;
}
