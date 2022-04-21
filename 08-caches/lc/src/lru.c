#include "lru.h"
#include "error.h"
#include "lruhash.h"
#include "memutil.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

cache_sl_node_t *cache_sl_node_init() {
  cache_sl_node_t *node;
  node = calloc_checked(1, sizeof(cache_sl_node_t));
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
  list = calloc_checked(1, sizeof(cache_sl_list_t));
  return list;
}

void cache_sl_list_free(cache_sl_list_t *list) {
  cache_sl_node_t *curr, *prev;
  curr = list->first;
  while (curr) {
    prev = curr;
    curr = curr->next;
    free(prev);
  }
  free(list);
}

void cache_sl_list_print(cache_sl_list_t *list) {
  cache_sl_node_t *curr;
  curr = list->first;
  while (curr) {
    printf("%lu ", curr->index);
    curr = curr->next;
  }
  printf("\n");
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

  if (list->len == 1) {
    list->first = list->last = NULL;

    list->len = 0;
    return node;
  }

  if (list->first == node) {
    list->first       = node->next;
    list->first->prev = NULL;
  } else if (list->last == node) {
    cache_sl_list_pop_back(list);
    return node;
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  list->len--;
  node->next = node->prev = NULL;
  return node;
}

struct lru_s {
  lru_hashtable_t *table;
  cache_sl_list_t *list;
  size_t           size, data_size, curr_top;
  void            *data;
  size_t           cache_hits;
  cache_get_func_t get_func;
};

lru_t *lru_init(size_t size, size_t data_size, cache_get_func_t get_func) {
  lru_t *cache;

  cache        = calloc_checked(1, sizeof(lru_t));
  cache->table = lru_hashtable_init(2 * size);

  cache->size      = size;
  cache->data_size = data_size;
  cache->get_func  = get_func;
  cache->data      = calloc_checked(size, data_size);

  cache->list = cache_sl_list_init();
  return cache;
}

void *lru_get(lru_t *cache, size_t index) {
  void            *data;
  cache_sl_node_t *node, *found;

  char *curr_data_ptr;

  found = lru_hashtable_lookup(cache->table, index);
  if (found) {
    cache->cache_hits++;
    cache_sl_list_remove(cache->list, found);
    cache_sl_list_push_front(cache->list, found);
    return found->data;
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
    lru_hashtable_remove(cache->table, node->index);
    memcpy(curr_data_ptr, data, cache->data_size);
    cache_sl_list_push_front(cache->list, node);
  }

  node->index = index;

  lru_hashtable_insert(cache->table, index, node);

  return data;
}

void lru_free(lru_t *cache) {
  ASSERTION(cache);

  lru_hashtable_free(cache->table);
  cache_sl_list_free(cache->list);

  free(cache->data);
  free(cache);
}

size_t lru_get_hits(lru_t *cache) {
  return cache->cache_hits;
}