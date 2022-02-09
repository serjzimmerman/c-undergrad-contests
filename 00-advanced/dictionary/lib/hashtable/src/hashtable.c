/* Asserts only in debug build type, in release mode pointers are assumed to be valid */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "sllistc.h"

struct pair_t {
  void *key;
  pair_val_t value;
};

struct chain_pointer_t {
  struct sl_node_t *node;
#ifdef USE_CHAIN_POINTER_N_OPTIMIZATION
  size_t n;
#endif
};

struct hash_table_t {
  struct sl_list_t *list;
  size_t size, buckets_used, inserts;
  size_t collisions;
  unsigned long (*hash_func)(const char *);
  struct chain_pointer_t array[];
};

/* djb2 hash function http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash_djb2(const char *str) {
  unsigned long hash = 5381;
  int c;

  assert(str);

  while ((c = *(str++))) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

struct pair_t *pair_init(const char *key, pair_val_t value) {
  struct pair_t *pair;
  size_t len;

  assert(key);

  if (!key) {
    return NULL;
  }

  pair = calloc(1, sizeof(struct pair_t));

  if (!pair) {
    return NULL;
  }

  len = strlen(key);
  pair->key = calloc(1, (len + 1) * sizeof(char));

  if (!pair->key) {
    free(pair);
    return NULL;
  }

  memcpy(pair->key, key, len);
  pair->value = value;

  return pair;
}

void pair_free(struct pair_t *pair) {
  assert(pair);

  free(pair->key);
  free(pair);
}

void pair_set_value(struct pair_t *pair, pair_val_t value) {
  assert(pair);

  pair->value = value;
}

pair_val_t pair_get_value(struct pair_t *pair) {
  assert(pair);

  return pair->value;
}

const char *pair_get_key(struct pair_t *pair) {
  assert(pair);

  return pair->key;
}

struct hash_table_t *hash_table_init(size_t size, unsigned long (*hash)(const char *)) {
  struct hash_table_t *table;

  assert(size);

  table = calloc(1, sizeof(struct hash_table_t) + sizeof(struct chain_pointer_t) * size);

  if (!table) {
    return NULL;
  }

  if (hash) {
    table->hash_func = hash;
  } else {
    table->hash_func = hash_djb2;
  }

  table->size = size;
  table->buckets_used = 0;
  table->collisions = 0;
  table->inserts = 0;
  table->list = sl_list_init();

  if (!table->list) {
    free(table);
    return NULL;
  }

  return table;
}

void hash_table_free(struct hash_table_t *table) {
  assert(table);

  sl_list_free(table->list, ((void *)(void *)pair_free));
  free(table);
}

size_t hash_table_get_size(struct hash_table_t *table) {
  assert(table);

  return table->size;
}

size_t hash_table_get_buckets_used(struct hash_table_t *table) {
  assert(table);

  return table->buckets_used;
}

size_t hash_table_get_collisions(struct hash_table_t *table) {
  assert(table);

  return table->collisions;
}

size_t hash_table_get_inserts(struct hash_table_t *table) {
  assert(table);

  return table->inserts;
}

#define HASH_TABLE_LOAD_FACTOR 0.7f

int hash_table_insert(struct hash_table_t **table, struct pair_t *pair) {
  struct sl_node_t *node;
  unsigned long hash;
  float load_factor;

  assert(table);
  assert(pair);

  load_factor = (float)(*table)->inserts / (*table)->size;

  if (load_factor > HASH_TABLE_LOAD_FACTOR) {
    (*table) = hash_table_resize(table, (*table)->size * 2);
  }

  if (!(*table)) {
    return 1;
  }

  hash = (*table)->hash_func(pair->key) % ((*table)->size);
  node = sl_node_init();
  if (!node) {
    return 1;
  }

  (*table)->inserts++;
  sl_node_set_data(node, pair);

  if ((*table)->array[hash].node == NULL) {
    sl_list_append((*table)->list, node);
    (*table)->array[hash].node = node;
#ifdef USE_CHAIN_POINTER_N_OPTIMIZATION
    (*table)->array[hash].n = 1;
#endif
    (*table)->buckets_used++;
    return 0;
  }

  sl_list_insert_after((*table)->list, (*table)->array[hash].node, node);
  (*table)->collisions++;
#ifdef USE_CHAIN_POINTER_N_OPTIMIZATION
  (*table)->array[hash].n++;
#endif

  return 0;
}

struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key) {
  struct sl_node_t *find;
  unsigned long hash;
#ifndef USE_CHAIN_POINTER_N_OPTIMIZATION
  unsigned long temphash;
#else
  int i;
#endif

  assert(table);
  assert(key);

#ifndef USE_CHAIN_POINTER_N_OPTIMIZATION
  hash = temphash = table->hash_func(key) % (table->size);
#else
  hash = table->hash_func(key) % (table->size);
#endif

  find = table->array[hash].node;

  if (!find) {
    return NULL;
  }

  /* If a node is found then linked list is seached for a node with specified key */
#ifndef USE_CHAIN_POINTER_N_OPTIMIZATION
  while (temphash == hash) {
    if (strcmp(((struct pair_t *)sl_node_get_data(find))->key, key) == 0) {
      return (struct pair_t *)sl_node_get_data(find);
    }
    find = sl_node_get_n_next(find, 1);
    if (!find) {
      break;
    }
    temphash = table->hash_func(((struct pair_t *)sl_node_get_data(find))->key) % (table->size);
  }
#else
  for (i = table->array[hash].n; i > 0; i--) {
    if (strcmp(((struct pair_t *)sl_node_get_data(find))->key, key) == 0) {
      return (struct pair_t *)sl_node_get_data(find);
    }
    find = sl_node_get_n_next(find, 1);
  }
#endif

  return NULL;
}

void resize_node_callback(struct sl_node_t *node, struct sl_list_t *list, va_list argp) {
  struct hash_table_t *table;
  unsigned long hash;

  table = va_arg(argp, struct hash_table_t *);
  hash = table->hash_func(((struct pair_t *)sl_node_get_data(node))->key) % (table->size);
  sl_list_remove_node(list, NULL, node);

  if (table->array[hash].node == NULL) {
    sl_list_push(table->list, node);
    table->array[hash].node = node;
#ifdef USE_CHAIN_POINTER_N_OPTIMIZATION
    table->array[hash].n = 1;
#endif
    table->buckets_used++;
    return;
  }

#ifdef USE_CHAIN_POINTER_N_OPTIMIZATION
  table->array[hash].n++;
#endif
  sl_list_insert_after(table->list, table->array[hash].node, node);
  table->collisions++;
}

struct hash_table_t *hash_table_resize(struct hash_table_t **table, size_t size) {
  struct sl_list_t *old;

  assert(table);
  assert(size);

  (*table)->size = size;
  old = (*table)->list;
  (*table) = realloc(*table, sizeof(struct hash_table_t) + sizeof(struct chain_pointer_t) * (*table)->size);

  if (!(*table)) {
    sl_list_free(old, ((void *)pair_free));
    return NULL;
  }

  (*table)->list = sl_list_init();
  if (!(*table)->list) {
    sl_list_free(old, (void *)pair_free);
    free(*table);
  }

  memset((*table)->array, 0, sizeof(struct chain_pointer_t) * size);
  (*table)->buckets_used = 0;
  (*table)->collisions = 0;

  sl_list_iterate_over_nodes(old, resize_node_callback, (*table), old);
  sl_list_free(old, (void *)(void *)pair_free);

  return (*table);
}

struct sl_list_t *hash_table_get_linked_list(struct hash_table_t *table, int free_table) {
  struct sl_list_t *list;

  assert(table);

  list = table->list;
  if (free_table) {
    free(table);
  }

  return list;
}