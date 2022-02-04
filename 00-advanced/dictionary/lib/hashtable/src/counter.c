#include "counter.h"
#include "hashtable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_COUNTER_SIZE 16
struct counter_t {
  struct hash_table_t *table;
};

struct counter_t *counter_init(unsigned long (*hash)(const char *)) {
  struct counter_t *counter;

  counter = calloc(1, sizeof(struct counter_t));
  assert(counter);

  counter->table = hash_table_init(DEFAULT_COUNTER_SIZE, hash);
  assert(counter->table);

  return counter;
}

void counter_free(struct counter_t *counter) {
  assert(counter);

  hash_table_free(counter->table);

  free(counter);
}

void counter_item_add(struct counter_t *counter, char *key) {
  struct pair_t *pair;

  pair = hash_table_lookup(counter->table, key);

  if (pair) {
    pair_set_value(pair, pair_get_value(pair) + 1);
  } else {
    pair = pair_init(key, 1);
    hash_table_insert(&counter->table, pair);
  }
}

unsigned counter_item_get_count(struct counter_t *counter, char *key) {
  struct pair_t *pair;

  pair = hash_table_lookup(counter->table, key);

  if (pair) {
    return pair_get_value(pair);
  } else {
    return 0;
  }
}

struct hash_table_t *counter_get_hashtable(struct counter_t *counter) {
  assert(counter);

  return counter->table;
}