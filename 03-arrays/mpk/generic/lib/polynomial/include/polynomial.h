#include <stdio.h>

typedef struct {
  int *coefficients;
  int len;
} polynomial_t;

struct int_stack_s {
  int *array, pos, len;
};

typedef struct int_stack_s int_stack_t;

polynomial_t *polynomial_init_malloc(const int len);
polynomial_t *polynomial_mul_karatsuba(polynomial_t * const a, polynomial_t * const b);

void polynomial_free(polynomial_t *poly);
void polynomial_print(const polynomial_t * const a);