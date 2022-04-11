int isprime(unsigned x) {
  int i = x - 1;

  if (x == 2) {
    return 1;
  }

  while (i >= 2) {
    if (x % (i--) == 0) {
      return 0;
    }
  }

  return 1;
}