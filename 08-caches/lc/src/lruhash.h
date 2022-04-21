#ifndef LRU_HASH_H
#define LRU_HASH_H

#include "lru.h"
#include <stddef.h>

typedef struct lru_hash_node_s {
  struct lru_hash_node_s *next;
  size_t                  index;
  cache_sl_node_t        *node;
} lru_hash_node_t;

typedef struct {
  unsigned          size, inserts;
  lru_hash_node_t **array;
} lru_hashtable_t;

lru_hashtable_t *lru_hashtable_init(unsigned size);
cache_sl_node_t *lru_hashtable_lookup(lru_hashtable_t *table, size_t index);

void lru_hashtable_insert(lru_hashtable_t *table, size_t index, cache_sl_node_t *node);
void lru_hashtable_remove(lru_hashtable_t *table, size_t index);
void lru_hashtable_free(lru_hashtable_t *table);

#endif