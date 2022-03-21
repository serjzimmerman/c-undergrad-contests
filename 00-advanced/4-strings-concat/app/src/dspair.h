#ifndef DSPAIR_H
#define DSPAIR_H

/* Double string pair */
struct dspair_s {
  char *first, *second;
};

typedef void *dspair_t;

dspair_t      dspair_init(char *first, char *second);
unsigned long hash_djb2_double(const char *first, const char *second);
unsigned long dspair_hash(dspair_t pair);
void          dspair_free(dspair_t pair);
char         *dspair_advance_pointer(struct dspair_s *pair, int *n, char *ptr);
int           dspair_cmp(dspair_t first, dspair_t second);

#endif