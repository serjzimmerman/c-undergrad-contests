#include "counter.h"
#include "hashtable.h"
#include "spair.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_COUNTER_SIZE 16
struct counter_t {
  struct hash_table_t *table;
};

struct counter_t *counter_init() {
  struct counter_t *counter;

  counter = calloc(1, sizeof(struct counter_t));
  assert(counter);

  counter->table = hash_table_init(DEFAULT_COUNTER_SIZE, spair_hash_djb2, spair_cmp, spair_free);
  assert(counter->table);

  return counter;
}

void counter_free(struct counter_t *counter, int free_table) {
  assert(counter);

  if (free_table) {
    hash_table_free(counter->table);
  }

  free(counter);
}

void counter_item_add(struct counter_t *counter, char *key) {
  struct spair_s find;
  spair_t        pair;
  find.key = key;

  pair = hash_table_lookup(counter->table, &find);

  if (pair) {
    spair_set_value(pair, spair_get_value(pair) + 1);
  } else {
    pair = spair_init(key, 1);
    hash_table_insert(&counter->table, pair);
  }
}

unsigned counter_item_get_count(struct counter_t *counter, char *key) {
  struct spair_s find;
  spair_t        pair;
  find.key = key;

  pair = hash_table_lookup(counter->table, &find);

  if (pair) {
    return spair_get_value(pair);
  } else {
    return 0;
  }
}

struct hash_table_t *counter_get_hashtable(struct counter_t *counter) {
  assert(counter);

  return counter->table;
}