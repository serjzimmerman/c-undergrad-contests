#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

struct ExtendedGcd {
	int a, b, g;
};

void swap(int *x, int *y){
	int temp;

	temp = *x;
	*x = *y;
	*y = temp;
}

int iabs(const int x) {
	return (x < 0) ? -x : x;
}

int eu_mod(int x, int y) {
	int r;

	assert(y != 0);

	r = x % y;

	if (r < 0 ) {
		r += iabs(y);
	}

	return r;
}

int eu_div(const int x, const int y) {
	assert(y != 0);

	return (x - eu_mod(x, y)) / y;
}

/* https://en.wikipedia.org/wiki/Extended_Euclidean_algorithm */
int extended_gcd(struct ExtendedGcd *egcd, const int x, const int y) {
	int old_r, r, old_s, s, old_t, t, q;

	old_r = x;
	old_s = 1;
	old_t = 0;
	r = y;
	s = 0;
	t = 1;

	assert(y != 0);

	while (r != 0) {
		q = eu_div(old_r, r);
		swap(&old_r, &r);
		swap(&old_s, &s);
		swap(&old_t, &t);
		r = r - q * old_r;
		s = s - q * old_s;
		t = t - q * old_t;
	}

	egcd->g = old_r;
	egcd->a = old_s;
	egcd->b = old_t;

	return 0;
}

int main() {
	int x, y, res;
  struct ExtendedGcd egcd;

	res = scanf("%d%d", &x, &y);

	if (res != 2 || y == 0) {
    printf("%s\n", "Wrong input");
    abort();
  }

  extended_gcd(&egcd, x, y);

	printf("%d %d %d\n", egcd.a, egcd.b, egcd.g);

	return 0;
}
