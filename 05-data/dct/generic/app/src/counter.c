#include "counter.h"
#define HASH_TABLE_USE_UINT
#include "hashtable.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_HASHTABLE_SIZE 16
#define HASH_TABLE_MAX_LOAD 0.75f

struct counter_t *counter_init() {
  struct counter_t *counter;

  counter = calloc(1, sizeof(struct counter_t));
  assert(counter);

  counter->table = hash_table_init(DEFAULT_HASHTABLE_SIZE);
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
  float load_factor;

  load_factor = (float)hash_table_get_buckets_used(counter->table) / hash_table_get_size(counter->table);

  if (load_factor > HASH_TABLE_MAX_LOAD) {
    counter->table = hash_table_resize(counter->table, hash_table_get_size(counter->table) * 2);
    assert(counter->table);
  }

  pair = hash_table_lookup(counter->table, key);

  if (pair) {
    pair->value = pair->value + 1;
  } else {
    pair = pair_init(key, 1);
    hash_table_insert(counter->table, pair);
  }
}

unsigned counter_item_get_count(struct counter_t *counter, char *key) {
  struct pair_t *pair;

  pair = hash_table_lookup(counter->table, key);

  if (pair) {
    return pair->value;
  } else {
    return 0;
  }
}