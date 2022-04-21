#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>

#define UNUSED_PARAMETER(expr) (void)(expr);

#if defined(NDEBUG) && (NDEBUG == 1)
#undef DEBUG
#else
#define DEBUG
#endif

#if defined(DEBUG)
#define MESSAGE(errtype, format, ...)                                                                                  \
  do {                                                                                                                 \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__,  \
            ##__VA_ARGS__);                                                                                            \
  } while (0)

#else
#define MESSAGE(format, ...)                                                                                           \
  do {                                                                                                                 \
    fprintf(stderr, format, ##__VA_ARGS__);                                                                            \
  } while (0)
#endif

#if defined(DEBUG)
#define WARNING(format, ...) MESSAGE("Warning", format, ##__VA_ARGS__);
#define ERROR(format, ...)                                                                                             \
  do {                                                                                                                 \
    MESSAGE("Fatal error", format, ##__VA_ARGS__);                                                                     \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)

#else
#define WARNING(format, ...)                                                                                           \
  do {                                                                                                                 \
  } while (0)
#define ERROR(format, ...)                                                                                             \
  do {                                                                                                                 \
    MESSAGE(format, ##__VA_ARGS__);                                                                                    \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)
#endif

#ifdef DEBUG
#define ASSERTION(condition)                                                                                           \
  if (!(condition)) {                                                                                                  \
    MESSAGE("Assertion (" #condition ") failed", "");                                                                  \
    exit(EXIT_FAILURE);                                                                                                \
  }
#else
#define ASSERTION(condition)                                                                                           \
  do {                                                                                                                 \
    UNUSED_PARAMETER(condition);                                                                                       \
  } while (0)
#endif

#endif

#ifndef MEMUTIL_H
#define MEMUTIL_H

#include <stdlib.h>

#if defined(DEBUG)
#define CALLOC_CHECKED(pointer, count, size)                                                                           \
  do {                                                                                                                 \
    (pointer) = calloc(count, size);                                                                                   \
    if (!(pointer)) {                                                                                                  \
      ERROR("Memory exhausted, cannot allocate enough memory of total size: %lu\n", (unsigned long)(count) * (size));  \
    }                                                                                                                  \
  } while (0)
#else
#define CALLOC_CHECKED(pointer, count, size)                                                                           \
  do {                                                                                                                 \
    (pointer) = calloc(count, size);                                                                                   \
    if (!(pointer)) {                                                                                                  \
      ERROR("Memory exhausted\n");                                                                                     \
    }                                                                                                                  \
  } while (0)
#endif

static void *calloc_checked(size_t count, size_t size) {
  void *ptr;
  ptr = calloc(count, size);
  if (!ptr) {
#ifdef NDEBUG
    ERROR("Memory exhausted\n");
#else
    ERROR("Memory exhausted, cannot allocate enough memory of total size: %lu\n", (unsigned long)(count) * (size));
#endif
  }

  return ptr;
}

#endif

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

#ifndef LRU_HASH_H
#define LRU_HASH_H

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

#include <stdint.h>
#include <stdlib.h>

unsigned long hash32bit(uint32_t a) {
  a = (a + 0x7ed55d16) + (a << 12);
  a = (a ^ 0xc761c23c) ^ (a >> 19);
  a = (a + 0x165667b1) + (a << 5);
  a = (a + 0xd3a2646c) ^ (a << 9);
  a = (a + 0xfd7046c5) + (a << 3);
  a = (a ^ 0xb55a4f09) ^ (a >> 16);
  return a;
}

lru_hashtable_t *lru_hashtable_init(unsigned size) {
  lru_hashtable_t *table;

  CALLOC_CHECKED(table, 1, sizeof(lru_hashtable_t));
  table->size = size;
  CALLOC_CHECKED(table->array, size, sizeof(lru_hash_node_t));

  return table;
}

lru_hash_node_t *lru_hash_node_init() {
  lru_hash_node_t *node;
  CALLOC_CHECKED(node, 1, sizeof(lru_hash_node_t));
  return node;
}

void lru_hashtable_insert(lru_hashtable_t *table, size_t index, cache_sl_node_t *node) {
  lru_hash_node_t *insert;
  unsigned long    hash;

  hash   = hash32bit(index) % table->size;
  insert = lru_hash_node_init();

  insert->node  = node;
  insert->index = index;

  table->inserts++;
  if (!table->array[hash]) {
    table->array[hash] = insert;
    return;
  }

  insert->next       = table->array[hash];
  table->array[hash] = insert;
}

void lru_hashtable_remove(lru_hashtable_t *table, size_t index) {
  lru_hash_node_t *curr, *prev, *found;
  unsigned long    hash = 0;

  hash = hash32bit(index) % table->size;

  if (!table->array[hash]) {
    ERROR("Trying to remove a nonexistent entry from hashtable\n");
  }

  if (table->array[hash]->index == index) {
    found              = table->array[hash];
    table->array[hash] = found->next;
    table->inserts--;
    free(found);
    return;
  }

  prev = table->array[hash];
  curr = prev->next;

  while (curr) {
    if (curr->index == index) {
      found      = curr;
      prev->next = found->next;
      table->inserts--;
      free(found);
      break;
    }
    prev = curr;
    curr = curr->next;
  }
}

cache_sl_node_t *lru_hashtable_lookup(lru_hashtable_t *table, size_t index) {
  lru_hash_node_t *found = NULL;
  unsigned long    hash  = 0;

  hash = hash32bit(index) % table->size;

  if (!table->array[hash]) {
    return NULL;
  }

  found = table->array[hash];
  while (found) {
    if (found->index == index) {
      return found->node;
    }
    found = found->next;
  }

  return NULL;
}

void lru_hashtable_free(lru_hashtable_t *table) {
  lru_hash_node_t *curr, *prev;
  size_t           i;

  for (i = 0; i < table->size; ++i) {
    curr = table->array[i];
    while (curr) {
      prev = curr;
      curr = curr->next;
      free(prev);
    }
  }

  free(table->array);
  free(table);
}

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

#include <stdlib.h>
#include <string.h>

void *slow_get_page(size_t index) {
  static int zero = 0;
  return &zero;
}

int main() {
  int    i, res = 0;
  size_t m, n, p = 0;
  lru_t *cache = NULL;

  res = scanf("%lu %lu", &m, &n);
  if (!res) {
    ERROR("Invalid input\n");
  }

  cache = lru_init(m, sizeof(int), slow_get_page);

  for (i = 0; i < n; ++i) {
    scanf("%lu", &p);
    lru_get(cache, p);
  }

  printf("%lu", lru_get_hits(cache));
  lru_free(cache);
}