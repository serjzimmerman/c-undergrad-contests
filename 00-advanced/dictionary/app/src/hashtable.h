#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stddef.h>
#include <stdint.h>

struct pair_t;
struct hash_table_t;

typedef unsigned pair_val_t;

struct pair_t *pair_init(const char *key, pair_val_t value);
void pair_free(struct pair_t *pair);
void pair_set_value(struct pair_t *pair, pair_val_t value);
pair_val_t pair_get_value(struct pair_t *pair);
struct hash_table_t *hash_table_init(size_t size, unsigned long (*hash)(const char *));
void hash_table_free(struct hash_table_t *table);
int hash_table_insert(struct hash_table_t *table, struct pair_t *pair);
struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key);
struct hash_table_t *hash_table_resize(struct hash_table_t **table, size_t size);
size_t hash_table_get_size(struct hash_table_t *table);
size_t hash_table_get_buckets_used(struct hash_table_t *table);
size_t hash_table_get_collisions(struct hash_table_t *table);

#endif