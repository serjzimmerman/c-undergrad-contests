#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

#define HASH_TABLE_USE_UINT
/* #define HASH_TABLE_USE_COLLISIONS */

typedef enum {
  HASH_TABLE_OK,
  HASH_TABLE_EALLOC,
  HASH_TABLE_INS_PRESENT,
} hash_table_err_t;

extern hash_table_err_t hash_table_errno;

#ifdef HASH_TABLE_USE_UINT
typedef unsigned pair_val_t;
#else
typedef int pair_val_t;
#endif

/* Key value is allocated on the stack */
struct pair_t {
  char *key;
  pair_val_t value;
  struct pair_t *next;
};

struct hash_table_t {
  struct pair_t **array;
  size_t size;
  size_t buckets_used;
#ifdef HASH_TABLE_USE_COLLISIONS
  size_t collisions;
#endif
};

struct pair_t *pair_init(char *key, pair_val_t value);
void pair_set_value(struct pair_t *pair, pair_val_t value);
pair_val_t pair_get_value(struct pair_t *pair);
struct hash_table_t *hash_table_init(size_t size);
void hash_table_free(struct hash_table_t *table);
int hash_table_insert(struct hash_table_t *table, struct pair_t *pair);
struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key);
struct hash_table_t *hash_table_resize(struct hash_table_t *table, size_t size);
size_t hash_table_get_size(struct hash_table_t *table);
size_t hash_table_get_buckets_used(struct hash_table_t *table);

#endif