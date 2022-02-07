#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intvl_t {
  int number;
  int start;
  int fin;
};

int intvl_cmp_by_fin(struct intvl_t *a, struct intvl_t *b) {
  return a->fin - b->fin;
}

void schedule_sort_by_fin(struct intvl_t *reqs, int nreqs) {
  qsort(reqs, nreqs, sizeof(struct intvl_t), (int (*)(const void *, const void *))intvl_cmp_by_fin);
}

int schedulemax(struct intvl_t *reqs, int nreqs) {
  struct intvl_t prev;
  int n = 1, i;

  if (nreqs == 0) {
    return 0;
  }

  schedule_sort_by_fin(reqs, nreqs);
  prev = reqs[0];

  for (i = 0; i < nreqs; i++) {
    if (reqs[i].start <= prev.fin) {
      continue;
    } else {
      prev = reqs[i];
      n++;
    }
  }

  return n;
}

int main() {
  return 0;
}