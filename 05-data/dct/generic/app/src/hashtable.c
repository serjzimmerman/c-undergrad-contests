#ifdef D_DEBUG
#define NDEBUG
#endif

/* Asserts only in debug build type, in release mode pointers are assumed to be valid */
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

hash_table_err_t hash_table_errno;
static struct hash_table_t *new_table_;

struct pair_t {
  char *key;
  pair_val_t value;
  struct pair_t *next;
};

struct hash_table_t {
  struct pair_t **array;
  size_t size;
  size_t buckets_used;
  size_t collisions;
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

struct pair_t *pair_init(char *key, pair_val_t value) {
  struct pair_t *pair;
  size_t len;

  assert(key);

  pair = calloc(1, sizeof(struct pair_t));
  assert(pair);

  if (!pair) {
    hash_table_errno = HASH_TABLE_EALLOC;
    return NULL;
  }

  len = strlen(key);

  pair->key = calloc(1, (len + 1) * sizeof(char));
  assert(pair->key);

  if (!pair->key) {
    hash_table_errno = HASH_TABLE_EALLOC;
    free(pair);
    return NULL;
  }

  memcpy(pair->key, key, len);
  pair->value = value;

  hash_table_errno = HASH_TABLE_OK;
  return pair;
}

void pair_set_value(struct pair_t *pair, pair_val_t value) {
  assert(pair);

  pair->value = value;
}

pair_val_t pair_get_value(struct pair_t *pair) {
  assert(pair);

  return pair->value;
}

void pair_free_list(struct pair_t *pair) {
  struct pair_t *next;

  assert(pair);

  while (pair) {
    next = pair->next;

    free(pair->key);
    free(pair);

    pair = next;
  }
}

void pair_free(struct pair_t *pair) {
  assert(pair);

  free(pair->key);
  free(pair);
}

struct hash_table_t *hash_table_init(size_t size) {
  struct hash_table_t *table;

  assert(size);

  table = calloc(1, sizeof(struct hash_table_t));
  assert(table);
  if (!table) {
    hash_table_errno = HASH_TABLE_EALLOC;
    return NULL;
  }

  table->size = size;
  table->buckets_used = 0;
  table->collisions = 0;
  table->array = calloc(size, sizeof(struct pair_t *));
  assert(table->array);

  if (!table->array) {
    hash_table_errno = HASH_TABLE_EALLOC;
    free(table);
    return NULL;
  }

  hash_table_errno = HASH_TABLE_OK;
  return table;
}

void hash_table_rehash_pair(struct pair_t *pair) {
  assert(new_table_);

  pair->next = NULL;
  hash_table_insert(new_table_, pair);
}

void hash_table_iterate_over_pairs(struct hash_table_t *table, void callback(struct pair_t *pair)) {
  struct pair_t *pair, *next;
  int i;

  assert(table);

  for (i = 0; i < table->size; i++) {
    if (table->array[i]) {
      pair = table->array[i];

      while (pair) {
        next = pair->next;
        callback(pair);
        pair = next;
      }
    }
  }
}

struct hash_table_t *hash_table_resize(struct hash_table_t *table, size_t size) {
  struct hash_table_t *new_table;

  assert(table);
  assert(size);

  new_table = hash_table_init(size);

  new_table->size = size;
  new_table_ = new_table;

  hash_table_iterate_over_pairs(table, hash_table_rehash_pair);

  free(table->array);
  free(table);

  return new_table;
}

void hash_table_free(struct hash_table_t *table) {
  size_t i;

  assert(table);

  hash_table_iterate_over_pairs(table, pair_free);

  free(table->array);
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
  struct pair_t *empty;
  unsigned long hash;

  assert(table);
  assert(pair);

  hash = hash_djb2(pair->key) % (table->size);

  empty = table->array[hash];

  if (empty == NULL) {
    table->array[hash] = pair;
    table->buckets_used++;
    hash_table_errno = HASH_TABLE_OK;
    return 0;
  }

  while (empty->next) {
    if (strcmp(empty->key, pair->key) == 0) {
      hash_table_errno = HASH_TABLE_INS_PRESENT;
      return 1;
    }
    empty = empty->next;
  }

  empty->next = pair;
  table->collisions = table->collisions + 1;

  hash_table_errno = HASH_TABLE_OK;

  return 0;
}

struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key) {
  struct pair_t *find;
  unsigned long hash;

  assert(table);
  assert(key);

  hash = hash_djb2(key) % (table->size);

  find = table->array[hash];

  if (!find) {
    return NULL;
  }

  /* If a node is found then linked list is seached for a node with specified key */
  while (find) {
    if (strcmp(find->key, key) == 0) {
      return find;
    }
    find = find->next;
  }

  return NULL;
}