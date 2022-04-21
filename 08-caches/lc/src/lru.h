#ifndef LRU_H
#define LRU_H

struct lru_s;
typedef struct lru_s lru_t;

#include <stddef.h>

typedef void *(*cache_get_func_t)(size_t index);
lru_t *lru_init(size_t size, size_t data_size, cache_get_func_t get_func);
void  *lru_get(lru_t *cache, size_t index);
size_t lru_get_hits(lru_t *cache);
void   lru_free(lru_t *cache);

typedef struct cache_sl_node_s {
  struct cache_sl_node_s *next, *prev;

  void  *data;
  size_t data_index, index;
} cache_sl_node_t;

#endif