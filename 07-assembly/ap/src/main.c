#include <stdio.h>

int main() {
  for (int i = 0; i < 128; ++i) {
    printf("%d - %d\n", i, isprime(i));
  }
}