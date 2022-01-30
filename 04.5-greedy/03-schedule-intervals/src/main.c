#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct intvl_t {
  int number;
  int start;
  int fin;
};

int intvl_cmp_by_fin(struct intvl_t *a, struct intvl_t *b) { return a->fin - b->fin; }

void schedule_sort_by_fin(struct intvl_t *reqs, int nreqs) {
  qsort(reqs, nreqs, sizeof(struct intvl_t), (int (*)(const void *, const void *))intvl_cmp_by_fin);
}

int schedulemax(struct intvl_t *reqs, int nreqs) {}

int main() { return 0; }