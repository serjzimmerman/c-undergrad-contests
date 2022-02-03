#ifdef D_DEBUG
#define NDEBUG
#endif

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
};

struct hash_table_t {
  struct sl_list_t *list;
  size_t size, buckets_used;
  size_t collisions;
  unsigned long (*hash_func)(const char *);
  struct chain_pointer_t array[];
};

/* djb2 hash function http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash_djb2(const char *str) {
  unsigned long hash = 5381;
  int c;

  assert(str);

  while (c = *(str++)) {
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

int hash_table_insert(struct hash_table_t *table, struct pair_t *pair) {
  struct sl_node_t *node;
  unsigned long hash;

  assert(table);
  assert(pair);

  hash = table->hash_func(pair->key) % (table->size);

  node = sl_node_init();
  if (!node) {
    return 1;
  }
  sl_node_set_data(node, pair);

  if (table->array[hash].node == NULL) {
    sl_list_append(table->list, node);
    table->array[hash].node = node;
    table->buckets_used++;
    return 0;
  }

  /* collision = sl_node_get_n_next(table->array[hash].node, 0); */
  sl_list_insert_after(table->list, table->array[hash].node, node);
  table->collisions++;
  /* table->array[hash].n++; */

  return 0;
}

struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key) {
  struct sl_node_t *find;
  unsigned long hash, temphash;
  int i;

  assert(table);
  assert(key);

  hash = temphash = table->hash_func(key) % (table->size);
  find = table->array[hash].node;

  if (!find) {
    return NULL;
  }

  /* If a node is found then linked list is seached for a node with specified key */
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
    table->buckets_used++;
    return;
  }

  sl_list_insert_after(table->list, table->array[hash].node, node);
  table->collisions++;
}

struct hash_table_t *hash_table_resize(struct hash_table_t **table, size_t size) {
  struct hash_table_t *new_table;
  struct sl_list_t *old;

  assert(table);
  assert(size);

  (*table)->size = size;
  old = (*table)->list;
  (*table) = realloc(*table, sizeof(struct hash_table_t) + sizeof(struct chain_pointer_t) * (*table)->size);

  if (!(*table)) {
    sl_list_free(old, ((void *)(void *)pair_free));
    return NULL;
  }

  (*table)->list = sl_list_init();

  memset((*table)->array, 0, sizeof(struct chain_pointer_t) * size);
  (*table)->buckets_used = 0;
  (*table)->collisions = 0;

  sl_list_iterate_over_nodes(old, resize_node_callback, (*table), old);
  sl_list_free(old, (void *)(void *)pair_free);

  return (*table);
}