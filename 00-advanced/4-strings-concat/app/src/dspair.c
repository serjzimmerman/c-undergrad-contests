#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "dspair.h"

dspair_t dspair_init(char *first, char *second) {
  struct dspair_s *pair;

  pair = calloc(1, sizeof(struct dspair_s));

  if (!pair) {
    return NULL;
  }

  pair->first  = first;
  pair->second = second;

  return (void *)pair;
}

unsigned long hash_djb2_double(const char *first, const char *second) {
  unsigned long hash = 5381;
  int           c;

  assert(first);
  assert(second);

  while ((c = *(first++))) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  while ((c = *(second++))) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

char *dspair_advance_pointer(struct dspair_s *pair, int *n, char *ptr) {
  if (!(*(ptr++ + 1)) && (*n) == 0) {
    *n = 1;
    return pair->second;
  }

  return ptr;
}

int dspair_cmp(dspair_t first, dspair_t second) {
  struct dspair_s *ds1, *ds2;
  char            *ptr1, *ptr2;
  int              n1 = 0, n2 = 0;

  ds1 = (struct dspair_s *)first;
  ds2 = (struct dspair_s *)second;

  ptr1 = ds1->first;
  ptr2 = ds2->first;

  while (*ptr1) {
    if (*ptr1 != *ptr2) {
      return *ptr1 - *ptr2;
    }

    ptr1 = dspair_advance_pointer(ds1, &n1, ptr1);
    ptr2 = dspair_advance_pointer(ds2, &n2, ptr2);
  }

  return 0;
}

unsigned long dspair_hash(dspair_t pair) {
  struct dspair_s *ds = (struct dspair_s *)pair;

  return hash_djb2_double(ds->first, ds->second);
}

void dspair_free(dspair_t pair) {
  free(pair);
}