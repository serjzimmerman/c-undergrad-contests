#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

struct Window {
  int min_x, max_x, min_y, max_y;
};

struct Facade {
  int n, m, window_count;
  struct Window *window_array;
};

int windows_overlap(struct Window *w1, struct Window *w2) {
  if (w1->max_x <= w2->min_x || w2->max_x <= w1->min_x) {
    return 0;
  }
  if (w1->max_y <= w1->min_y || w1->max_y <= w1->min_y) {
    return 0;
  }

  return 1;
}

int window_inside(struct Window *w1, struct Facade *f1) {
  if (w1->max_y > f1->m || w1->min_y < 0) {
    return 0;
  }
  if (w1->max_x > f1->n || w1->min_x < 0) {
    return 0;
  }

  return 1;
}

int main() {
  int temp_count = 0, correct = 1, i, j;
  struct Facade facade = {0};
  struct Window *array;

  scanf("%d %d", &facade.n, &facade.m);

  scanf("%d", &temp_count);
  array = (struct Window *)calloc(sizeof(struct Window) * temp_count, 1);

  for (i = 0; i < temp_count; i++) {
    scanf("%d %d %d %d", &array[i].min_x, &array[i].max_x, &array[i].min_y,
          &array[i].max_y);
  }

  facade.window_array = array;
  facade.window_count = temp_count;

  for (i = 0; i < temp_count; i++) {
    if (window_inside(&array[i], &facade) == 0) {
      correct = 0;
    }
    for (j = 0; j < temp_count; j++) {
      if (j == i) {
        continue;
      }
      if (windows_overlap(&array[i], &array[j])) {
        correct = 0;
      }
    }
  }

  free(array);
  printf("%s", (correct ? "correct\n" : "broken\n"));

  return 0;
}
