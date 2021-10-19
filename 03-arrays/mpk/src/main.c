#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

typedef struct {
  int *coefficients;
  int len;
} polynomial;

polynomial *polynomial_init_calloc(int len) {
  polynomial *new;

  if (!(new = (polynomial *)malloc(sizeof(polynomial)))) {
    abort();
  }

  new->len = len;

  if (!(new->coefficients = (int *)calloc(len, sizeof(int)))) {
    abort();
  }

  return new;
}

polynomial *polynomial_init_malloc(int len) {
  polynomial *new;

  if (!(new = (polynomial *)malloc(sizeof(polynomial)))) {
    abort();
  }

  new->len = len;

  if (!(new->coefficients = (int *)malloc(len * sizeof(int)))) {
    abort();
  }

  return new;
}

polynomial *polynomial_create(int *array, int len) {
  int i;

  polynomial *poly = polynomial_init_malloc(len);

  for (i = 0; i < poly->len; i++) {
    poly->coefficients[i] = array[i];
  }

  return poly;
}

polynomial *polynomial_copy(polynomial *copy_from) {
  int i;

  polynomial *poly = malloc(sizeof(polynomial));

  poly->coefficients = copy_from->coefficients;
  poly->len = copy_from->len;

  return poly;
}

void polynomial_free(polynomial *poly) {
  free(poly->coefficients);
  free(poly);
}

void polynomial_split_half(polynomial *a, polynomial *a0, polynomial *a1) {
  a0->coefficients = &a->coefficients[0];
  a1->coefficients = &a->coefficients[a->len >> 1];

  a0->len = a->len >> 1;
  a1->len = a->len >> 1;
}

void polynomial_sum_of_two(polynomial *r, polynomial *a, polynomial *b) {
  int i;
  
  for (i = 0; i < a->len; i++) {
    r->coefficients[i] = a->coefficients[i] + b->coefficients[i];
  }
}

void polynomial_sum_of_two_double(polynomial *r1, polynomial *a1, polynomial *b1, polynomial *r2, polynomial *a2, polynomial *b2) {
  int i;
  
  for (i = 0; i < a1->len; i++) {
    r1->coefficients[i] = a1->coefficients[i] + b1->coefficients[i];
    r2->coefficients[i] = a2->coefficients[i] + b2->coefficients[i];
  }
}

void polynomial_sum_to_one(polynomial *r, polynomial *a) {
  int i;
  
  for (i = 0; i < a->len; i++) {
    r->coefficients[i] += a->coefficients[i];
  }
}

void polynomial_diff_from_two(polynomial *r, polynomial *a, polynomial *b) {
  int i;
  
  for (i = 0; i < a->len; i++) {
    r->coefficients[i] -= a->coefficients[i] + b->coefficients[i];
  }
}

void polynomial_diff_from_one(polynomial *r, polynomial *a) {
  int i;
  
  for (i = 0; i < a->len; i++) {
    r->coefficients[i] -= a->coefficients[i];
  }
}

polynomial *temp_data (int n, polynomial *suma, polynomial *sumb, polynomial *mul, polynomial *temp) {
  /* temp = polynomial_init_malloc(n << 2); */

  if (temp->len < (n << 2)) {
    abort();
  }

  suma->len = n;
  sumb->len = n;

  suma->coefficients = &temp->coefficients[0];
  sumb->coefficients = &temp->coefficients[n];

  mul->len = n << 1;
  mul->coefficients = &temp->coefficients[n << 1];

  temp->coefficients = mul->coefficients[n << 1];
  temp->len -= n << 2;

  return temp;
}

/* Works only for polynomial of equal size 2^n */
void *polynomial_mul_karatsuba_impl(polynomial *r, polynomial *a, polynomial *b, polynomial *temp) {
  polynomial a1, a0, b1, b0, a0b0, a1b1, mul, suma, sumb, middle;

  /* if (a->len == 1) {
    r->coefficients[0] = a->coefficients[0] * b->coefficients[0];
    r->coefficients[1] = 0;
    return;
  } */

  if (a->len == 2) {
    r->coefficients[0] = a->coefficients[0] * b->coefficients[0];
    r->coefficients[1] = a->coefficients[0] * b->coefficients[1] + a->coefficients[1] * b->coefficients[0];;
    r->coefficients[2] = a->coefficients[1] * b->coefficients[1];
    r->coefficients[3] = 0;
    return;
  }

  /* Check whether a->len == b->len && a->len = 2^x */

  polynomial_split_half(a, &a0, &a1);
  polynomial_split_half(b, &b0, &b1);
  polynomial_split_half(r, &a0b0, &a1b1);

  polynomial_mul_karatsuba_impl(&a0b0, &a0, &b0, temp);
  polynomial_mul_karatsuba_impl(&a1b1, &a1, &b1, temp);

  /* This is a sort of "stack" to avoid multiple allocation calls */
  temp = temp_data(a0.len, &suma, &sumb, &mul, temp);
  temp->coefficients = &mul.coefficients[a->len];

  polynomial_sum_of_two_double(&sumb, &b0, &b1, &suma, &a0, &a1);
  polynomial_mul_karatsuba_impl(&mul, &suma, &sumb, temp);

  middle.coefficients = &(r->coefficients[a0.len]);
  middle.len = a->len;

  polynomial_diff_from_two(&mul, &a0b0, &a1b1);
  polynomial_sum_to_one(&middle, &mul);

  temp->coefficients -= a->len << 1;
  temp->len += a->len << 1;
}

#define ALLOCATION_SIZE 10000000

polynomial *polynomial_mul_karatsuba(polynomial *a, polynomial *b) {
  polynomial *r, *temp, *old;

  r = polynomial_init_malloc(a->len + b->len);

  /* Approximate maximum memory usage instead of a magic number */
  temp = polynomial_init_malloc(ALLOCATION_SIZE);
  old = polynomial_copy(temp);

  polynomial_mul_karatsuba_impl(r, a, b, temp);

  polynomial_free(old);
  free(temp);

  return r;
}

void polynomial_print(polynomial *a) {
  int i, j;

  for (j = a->len - 1; a->coefficients[j] == 0; j--) {
    ;
  }

  for (i = 0; i <= j; i++) {
    printf("%d ", a->coefficients[i]);
  }

  printf("\n");
}

int main() {
  int i, n, m, *array1, *array2;
  polynomial *p1, *p2, *p3;

  scanf("%d %d", &n, &m);

  array1 = malloc(n * sizeof(int));
  array2 = malloc(m * sizeof(int));

  for (i = 0; i < n; i++) {
    scanf("%d", &array1[i]);
  }
  for (i = 0; i < m; i++) {
    scanf("%d", &array2[i]);
  }

  p1 = malloc(sizeof(polynomial));
  p2 = malloc(sizeof(polynomial));

  p1->len = n;
  p2->len = m;
  p1->coefficients = array1;
  p2->coefficients = array2;

  p3 = polynomial_mul_karatsuba(p1, p2);

  polynomial_print(p3);
  polynomial_free(p3);
  
  free(p1);
  free(p2);
  free(array1);
  free(array2);

  return 0;
}
