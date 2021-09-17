#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

void swap(unsigned *x, unsigned *y){
	unsigned temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

unsigned fib_modulo(unsigned x, unsigned n) {
	unsigned i, r, r_old, r_old_old, temp;

	if (x < 2) {
		return x;
	}

	r_old_old = 0;
	r_old = 1;
	r = 1;
	i = 1;

	while (i++ < x) {
		temp = r;
		r = (r_old_old + r_old) % n;
		swap(&temp, &r_old);
		r_old_old = r;
	}

	return r;
}

int main() {
	unsigned x, y, res;

	res = scanf("%u%u", &x, &y);

	if (res != 2 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

	printf("%u\n", fib_modulo(x, y));

	return 0;
}
