#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/polynomial.h"

#define USE_BUILTIN_CLZ

#define max(a, b) (a > b ? a : b)

int msb(int a) {
  int c = 0;

  if (!a) {
    return -1;
  }

#ifdef USE_BUILTIN_CLZ
  return 31 - __builtin_clz(a);
#else
  while (a >>= 1) {
    c++;
  }
  return c;
#endif
}

polynomial_t *polynomial_init_malloc(const int len) {
  polynomial_t *new;

  if (!(new = (polynomial_t *)malloc(sizeof(polynomial_t)))) {
    return NULL;
  }

  new->len = len;

  if (!(new->coefficients = (int *)malloc(len * sizeof(int)))) {
    free(new);
    return NULL;
  }

  return new;
}

polynomial_t *polynomial_expand(polynomial_t * const poly, const int new_len) {
  if (!(poly->coefficients = (int *)realloc(poly->coefficients, new_len * sizeof(int)))) {
    return NULL;
  }
  
  memset(&poly->coefficients[poly->len], 0, (new_len - poly->len) * sizeof(int));

  poly->len = new_len;

  return poly;
}

void polynomial_free(polynomial_t *poly) {
  free(poly->coefficients);
  free(poly);
}

void polynomial_split_half(const polynomial_t * const a, polynomial_t * const a0, polynomial_t * const a1) {
  a0->coefficients = &a->coefficients[0];
  a1->coefficients = &a->coefficients[a->len >> 1];

  a0->len = a->len >> 1;
  a1->len = a->len >> 1;
}

void polynomial_sum_of_two_double(polynomial_t * const r1, const polynomial_t *a1,
                                  const polynomial_t * const b1, polynomial_t * const r2,
                                  const polynomial_t * const a2, const polynomial_t * const b2) {
  int i;

  for (i = 0; i < a1->len; i++) {
    r1->coefficients[i] = a1->coefficients[i] + b1->coefficients[i];
    r2->coefficients[i] = a2->coefficients[i] + b2->coefficients[i];
  }
}

void polynomial_sum_to_one(polynomial_t * const r, const polynomial_t * const a) {
  int i;

  for (i = 0; i < a->len; i++) {
    r->coefficients[i] += a->coefficients[i];
  }
}

void polynomial_diff_from_two(polynomial_t *r, const polynomial_t *a, const polynomial_t *b) {
  int i;

  for (i = 0; i < a->len; i++) {
    r->coefficients[i] -= a->coefficients[i] + b->coefficients[i];
  }
}

int_stack_t *int_stack_expand(int_stack_t * const a) {
  if (!(a->array = realloc(a->array, (a->len * 2) * sizeof(int)))) {
    return NULL;
  }

  a->len = a->len * 2;

  return a;
}

void int_stack_temp_data(const int n, polynomial_t * const suma, polynomial_t * const sumb,
                      polynomial_t * const mul, int_stack_t * const temp) {
  /* Expand if the is no space left */
  while (temp->len - temp->pos <= (n << 2)) {
    int_stack_expand(temp);
  }

  suma->len = n;
  sumb->len = n;

  suma->coefficients = &temp->array[temp->pos];
  sumb->coefficients = &temp->array[temp->pos + n];

  mul->len = n << 1;
  mul->coefficients = &temp->array[temp->pos + (n << 1)];

  temp->pos = temp->pos + (n << 2);
}

/* Works only for polynomial_t of equal size 2^k */
void polynomial_mul_karatsuba_impl(polynomial_t * const r, const polynomial_t * const a, const polynomial_t * const b,
                                    int_stack_t * const temp) {
  polynomial_t a1, a0, b1, b0, a0b0, a1b1, mul, suma, sumb, middle;

  /* Hardcoded basecase of n = 2 reduces the number of recursive calls */
  if (a->len == 2) {
    r->coefficients[0] = a->coefficients[0] * b->coefficients[0];
    r->coefficients[1] = a->coefficients[0] * b->coefficients[1] +
                         a->coefficients[1] * b->coefficients[0];
    ;
    r->coefficients[2] = a->coefficients[1] * b->coefficients[1];
    /* Necessary to set to 0 because heap contains junk from previous function
     * calls */
    r->coefficients[3] = 0;
    return;
  }

  /* Check whether a->len == b->len && a->len = 2^x */
  polynomial_split_half(a, &a0, &a1);
  polynomial_split_half(b, &b0, &b1);
  polynomial_split_half(r, &a0b0, &a1b1);

  /* Heap memory is necessary */
  polynomial_mul_karatsuba_impl(&a0b0, &a0, &b0, temp);
  polynomial_mul_karatsuba_impl(&a1b1, &a1, &b1, temp);

  /* This is a sort of "stack" to avoid multiple allocation calls */
  int_stack_temp_data(a0.len, &suma, &sumb, &mul, temp);

  polynomial_sum_of_two_double(&sumb, &b0, &b1, &suma, &a0, &a1);
  polynomial_mul_karatsuba_impl(&mul, &suma, &sumb, temp);

  middle.coefficients = &(r->coefficients[a0.len]);
  middle.len = a->len;

  polynomial_diff_from_two(&mul, &a0b0, &a1b1);
  polynomial_sum_to_one(&middle, &mul);

  temp->pos -= a->len << 1;
}

void int_stack_free(int_stack_t *a) {
  free(a->array);
  free(a);
}

int_stack_t *int_stack_init() {
  int_stack_t *r;

  if (!(r = (int_stack_t *)calloc(1, sizeof(int_stack_t)))) {
    return NULL;
  };

  r->len = 32768;

  if (!(r->array = (int *)malloc(r->len * sizeof(int)))) {
    free(r);
    return NULL;
  }

  return r;
}

int is_pow_of_two(const int a) {
  return ((a & (a - 1)) == 0);
}

polynomial_t *polynomial_mul_karatsuba(polynomial_t * const a, polynomial_t * const b) {
  polynomial_t *r;
  int_stack_t *temp;
  int align_size;

  if (!(a->len == b->len && is_pow_of_two(a->len) && is_pow_of_two(b->len))) {
    if (a->len > b->len && is_pow_of_two(a->len)) {
      align_size = a->len;
    } else if (b->len > a->len && is_pow_of_two(b->len)) {
      align_size = b->len;
    } else {
      align_size = 1 << (max(msb(a->len), msb(b->len)) + 1);
    }

    if (!((polynomial_expand(a, align_size)) && (polynomial_expand(b, align_size)))) {
      return NULL;
    }
  }

  if (!(r = polynomial_init_malloc(a->len + b->len))) {
    return NULL;
  }

  if (!(temp = int_stack_init())) {
    free(r);
    return NULL;
  }

  polynomial_mul_karatsuba_impl(r, a, b, temp);

  int_stack_free(temp);

  return r;
}

void polynomial_print(const polynomial_t * const a) {
  int i, j;

  for (j = a->len - 1; a->coefficients[j] == 0; j--) {
    ;
  }

  for (i = 0; i <= j; i++) {
    printf("%d ", a->coefficients[i]);
  }

  printf("\n");
}
