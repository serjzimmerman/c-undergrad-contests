#include <assert.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int *coefficients;
  int len;
} polynomial;

polynomial *polynomial_init(int len) {
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

polynomial *polynomial_create(int *array, int len) {
  int i;

  polynomial *poly = polynomial_init(len);

  for (i = 0; i < poly->len; i++) {
    poly->coefficients[i] = array[i];
  }

  return poly;
}

void polynomial_free(polynomial *poly) {
  free(poly->coefficients);
  free(poly);
}

polynomial *polynomial_mul_pow_x(polynomial *a, int pow) {
  int i, *temp;

  temp = malloc((a->len) * sizeof(int));
  memcpy(temp, a->coefficients, a->len * sizeof(int));

  a->coefficients = realloc(a->coefficients, (a->len + pow) * sizeof(int));
  
  for (i = 0; i < a->len; i++) {
    a->coefficients[i + pow] = temp[i];
  }

  for (i = 0; i < pow; i++) {
    a->coefficients[i] = 0;
  }
  
  a->len = a->len + pow;
  
  free(temp);

  return a;
}

void polynomial_split_half(polynomial *a, polynomial *a0, polynomial *a1) {
  int i;

  a0->coefficients = &a->coefficients[0];
  a1->coefficients = &a->coefficients[a->len / 2];

  a0->len = a->len / 2;
  a1->len = a->len / 2;
}

polynomial *polynomial_sum(polynomial *a, polynomial *b) {
  int i;

  if (a->len < b->len) { 
    a->coefficients = realloc(a->coefficients, (b->len) * sizeof(int));
    for (i = 0; i < a->len; i++) {
      a->coefficients[i] = a->coefficients[i] + b->coefficients[i];
    }
    for (i = a->len; i < b->len; i++) {
      a->coefficients[i] = b->coefficients[i];
    }
    a->len = b->len;
  } else { 
    for (i = 0; i < b->len; i++) {
      a->coefficients[i] = a->coefficients[i] + b->coefficients[i];
    }
  }

  return a;
}

void polynomial_sum_to(polynomial *r, polynomial *a, polynomial *b) {
  polynomial_sum(r, a);
  polynomial_sum(r, b);
}

polynomial *polynomial_diff(polynomial *a, polynomial *b) {
  int i;
  
  if (a->len < b->len) { 
    a->coefficients = realloc(a->coefficients, (b->len) * sizeof(int));
    for (i = 0; i < a->len; i++) {
      a->coefficients[i] = a->coefficients[i] - b->coefficients[i];
    }
    for (i = a->len; i < b->len; i++) {
      a->coefficients[i] = -b->coefficients[i];
    }
    a->len = b->len;
  } else { 
    for (i = 0; i < b->len; i++) {
      a->coefficients[i] = a->coefficients[i] - b->coefficients[i];
    }
  }

  return a;
}

polynomial *polynomial_mul_karatsuba(polynomial *a, polynomial *b) {
  polynomial *r, a1, a0, b1, b0, *a0b0, *a1b1, *mul, *suma, *sumb;

  if (a->len == 1) {
    r = polynomial_init(a->len);

    r->coefficients[0] = a->coefficients[0] * b->coefficients[0];
    
    return r;
  }

  /* Check whether a->len == b->len && a->len = 2^x */

  polynomial_split_half(a, &a0, &a1);
  polynomial_split_half(b, &b0, &b1);

  a0b0 = polynomial_mul_karatsuba(&a0, &b0);
  a1b1 = polynomial_mul_karatsuba(&a1, &b1);

  suma = polynomial_init(a0.len);
  sumb = polynomial_init(b0.len);

  polynomial_sum_to(suma, &a0, &a1);
  polynomial_sum_to(sumb, &b0, &b1);

  mul = polynomial_mul_karatsuba(suma, sumb);

  polynomial_mul_pow_x(polynomial_diff(polynomial_diff(mul, a1b1), a0b0), a->len / 2);
  polynomial_mul_pow_x(a1b1, a->len);

  r = polynomial_sum(polynomial_sum(a1b1, a0b0), mul);

  polynomial_free(suma);
  polynomial_free(sumb);
  polynomial_free(a0b0);
  polynomial_free(mul);

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

  p1 = polynomial_create(&array1[0], n);
  p2 = polynomial_create(&array2[0], m);

  free(array1);
  free(array2);

  p3 = polynomial_mul_karatsuba(p1, p2);

  polynomial_print(p3);

  polynomial_free(p1);
  polynomial_free(p2);
  polynomial_free(p3);
  
  return 0;
}
