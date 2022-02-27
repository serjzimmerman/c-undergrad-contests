#ifndef PAIR_H
#define PAIR_H

typedef unsigned pair_val_t;

struct spair_s {
  void *key;
  pair_val_t value;
};

typedef void *spair_t;

unsigned long hash_djb2(const char *str);
unsigned long pair_hash_djb2(const void *pair);
spair_t pair_init(const char *key, pair_val_t value);
void pair_free(spair_t pair);
void pair_set_value(spair_t pair, pair_val_t value);
pair_val_t pair_get_value(spair_t pair);
const char *pair_get_key(spair_t pair);
int pair_cmp(spair_t a, spair_t b);

#endif