#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "sllistc.h"
#include "spair.h"

#include <stddef.h>
#include <stdint.h>

/* Incomplete type to provide handle to hashtable data type as a pointer to the struct */
struct hash_table_t;

struct hash_table_t *hash_table_init(size_t size, unsigned long (*hash)(const void *), int (*pair_cmp)(void *, void *),
                                     void (*pair_free)(void *));
void hash_table_free(struct hash_table_t *table);

int hash_table_insert(struct hash_table_t **table, void *pair);
void *hash_table_lookup(struct hash_table_t *table, void *key);
struct hash_table_t *hash_table_resize(struct hash_table_t **table, size_t size);

size_t hash_table_get_size(struct hash_table_t *table);
size_t hash_table_get_buckets_used(struct hash_table_t *table);
size_t hash_table_get_collisions(struct hash_table_t *table);
size_t hash_table_get_inserts(struct hash_table_t *table);

struct sl_list_t *hash_table_get_linked_list(struct hash_table_t *table, int free_table);

#endif