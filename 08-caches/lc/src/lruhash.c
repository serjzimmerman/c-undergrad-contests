#include "lruhash.h"
#include <stdint.h>
#include <stdlib.h>

#include "error.h"
#include "memutil.h"

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