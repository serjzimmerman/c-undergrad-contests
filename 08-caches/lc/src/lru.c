#include "lru.h"
#include "error.h"
#include "hashtable.h"
#include "memutil.h"
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct cache_sl_node_s {
  struct cache_sl_node_s *next, *prev;

  void  *data;
  size_t data_index;
} cache_sl_node_t;

cache_sl_node_t *cache_sl_node_init() {
  cache_sl_node_t *node;
  CALLOC_CHECKED(node, 1, sizeof(cache_sl_node_t));
  return node;
}

void cache_sl_node_free(cache_sl_node_t *node) {
  free(node);
}

typedef struct {
  cache_sl_node_t *first, *last;
  size_t           len;
} cache_sl_list_t;

cache_sl_list_t *cache_sl_list_init() {
  cache_sl_list_t *list;
  CALLOC_CHECKED(list, 1, sizeof(cache_sl_list_t));
  return list;
}

void cache_sl_list_push_front(cache_sl_list_t *list, cache_sl_node_t *node) {
  ASSERTION(list);
  ASSERTION(node);

  if (list->len == 0) {
    list->first = list->last = node;
    node->prev = node->next = NULL;
  } else {
    list->first->prev = node;
    node->next        = list->first;
    node->prev        = NULL;
    list->first       = node;
  }
  list->len++;
}

cache_sl_node_t *cache_sl_list_pop_back(cache_sl_list_t *list) {
  cache_sl_node_t *result;
  ASSERTION(list);

  if (list->len == 0) {
    return NULL;
  } else if (list->len == 1) {
    result      = list->first;
    list->first = NULL;
    return result;
  } else {
    result           = list->last;
    list->last       = result->prev;
    list->last->next = result->prev = NULL;
  }

  list->len--;
  return result;
}

cache_sl_node_t *cache_sl_list_remove(cache_sl_list_t *list, cache_sl_node_t *node) {
  ASSERTION(list);

  if (list->len == 0) {
    ERROR("Trying to remove a node from empty list\n");
  }

  if (list->first == node) {
    list->first       = node->next;
    list->first->prev = NULL;
  } else if (list->last == node) {
    cache_sl_list_pop_back(list);
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  list->len--;
  node->next = node->prev = NULL;
  return node;
}

typedef struct {
  uint32_t         index;
  cache_sl_node_t *node;
} hash_cache_entry_t;

unsigned long hash32bit(uint32_t a) {
  a = (a + 0x7ed55d16) + (a << 12);
  a = (a ^ 0xc761c23c) ^ (a >> 19);
  a = (a + 0x165667b1) + (a << 5);
  a = (a + 0xd3a2646c) ^ (a << 9);
  a = (a + 0xfd7046c5) + (a << 3);
  a = (a ^ 0xb55a4f09) ^ (a >> 16);
  return a;
}

unsigned long cache_entry_hash(const void *a) {
  hash_cache_entry_t *entry;
  uint32_t            index;
  index = ((hash_cache_entry_t *)a)->index;
  return hash32bit(index);
}

int cache_entry_cmp(void *a, void *b) {
  hash_cache_entry_t *first, *second;
  first  = (hash_cache_entry_t *)a;
  second = (hash_cache_entry_t *)b;

  return (int)(first->index - second->index);
}

struct lru_s {
  struct hash_table_t *table;
  cache_sl_list_t     *list;
  size_t               size, data_size, curr_top;
  void                *data;
  size_t               cache_hits;
  cache_get_func_t     get_func;
};

lru_t *lru_init(size_t size, size_t data_size, cache_get_func_t get_func) {
  lru_t *cache;

  CALLOC_CHECKED(cache, 1, sizeof(lru_t));
  cache->table = hash_table_init(2 * size, cache_entry_hash, cache_entry_cmp, free);

  cache->size      = size;
  cache->data_size = data_size;
  cache->get_func  = get_func;
  CALLOC_CHECKED(cache->data, size, data_size);

  cache->list = cache_sl_list_init();
  return cache;
}

void *lru_get(lru_t *cache, size_t index) {
  void              *data;
  hash_cache_entry_t key = {0}, *found;
  cache_sl_node_t   *node;

  char *curr_data_ptr;

  key.index = index;
  found     = hash_table_lookup(cache->table, &key);
  if (found) {
    cache->cache_hits++;
    cache_sl_list_push_front(cache->list, cache_sl_list_remove(cache->list, found->node));
    return found->node->data;
  }

  data = cache->get_func(index);
  if (cache->curr_top < cache->size) {
    curr_data_ptr = ((char *)cache->data + cache->data_size * cache->curr_top);
    memcpy(curr_data_ptr, data, cache->data_size);

    node             = cache_sl_node_init();
    node->data       = curr_data_ptr;
    node->data_index = cache->curr_top;
    cache_sl_list_push_front(cache->list, node);
    cache->curr_top++;
  } else {
    node          = cache_sl_list_pop_back(cache->list);
    curr_data_ptr = ((char *)cache->data + cache->data_size * node->data_index);
    memcpy(curr_data_ptr, data, cache->data_size);

    cache_sl_list_push_front(cache->list, node);
  }

  CALLOC_CHECKED(found, 1, sizeof(hash_cache_entry_t));
  found->index = index;
  found->node  = node;

  hash_table_insert(&cache->table, found);
  return data;
}

size_t lru_get_hits(lru_t *cache) {
  return cache->cache_hits;
}