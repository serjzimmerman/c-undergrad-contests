#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct student {
  float weight;
  float height;
};

int compare(const void *a, const void *b) {
  struct student *first;
  struct student *second;

  first = (struct student *)a;
  second = (struct student *)b;

  if (first->weight != second->weight) {
    return (first->weight > second->weight);
  } else {
    return (first->height < second->height);
  }

  return 0;
}

int main() {
  struct student *array;
  int n, i;

  scanf("%d", &n);

  array = calloc(n, sizeof(struct student));

  for (i = 0; i < n; i++) {
    scanf("%f %f", &(array[i].height), &(array[i].weight));
  }

  qsort(array, n, sizeof(struct student), compare);

  for (i = 0; i < n; i++) {
    printf("%.2f %.3f\n", array[i].height, array[i].weight);
  }

  free(array);

  return 0;
}
