#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Overflow collision resolution using singly linked lists */
struct pair_t {
  char          *key;
  int            value;
  struct pair_t *next;
};

struct hash_table_t {
  struct pair_t **array;
  size_t          size;
};

/* djb2 hash function http://www.cse.yorku.ca/~oz/hash.html */
unsigned long hash_djb2(const char *str) {
  unsigned long hash = 5381;
  int           c;

  while (c = *(str++)) {
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
  }

  return hash;
}

struct pair_t *pair_init(char *key, int value) {
  struct pair_t *pair;
  size_t         len;

  pair = calloc(1, sizeof(struct pair_t));
  assert(pair);

  len = strlen(key);

  pair->key = calloc(1, (len + 1) * sizeof(char));
  assert(pair->key);

  memcpy(pair->key, key, len);
  pair->value = value;

  return pair;
}

void pair_free(struct pair_t *pair) {
  struct pair_t *next;

  assert(pair);

  while (pair) {
    next = pair->next;

    free(pair->key);
    free(pair);

    pair = next;
  }
}

struct hash_table_t *hash_table_init(size_t size) {
  struct hash_table_t *table;

  table = calloc(1, sizeof(struct hash_table_t));
  assert(table);
  table->size = size;

  table->array = calloc(size, sizeof(struct pair_t *));
  assert(table->array);

  return table;
}

void hash_table_free(struct hash_table_t *table) {
  size_t i;

  assert(table);

  for (i = 0; i < table->size; i++) {
    if (table->array[i]) {
      pair_free(table->array[i]);
    }
  }

  free(table->array);
  free(table);
}

void hash_table_insert(struct hash_table_t *table, struct pair_t *pair) {
  struct pair_t *empty;
  unsigned long  hash;

  assert(table);
  assert(pair);

  hash = hash_djb2(pair->key) % (table->size);

  empty = table->array[hash];

  if (empty == NULL) {
    table->array[hash] = pair;
    return;
  }

  while (empty->next) {
    empty = empty->next;
  }

  empty->next = pair;
}

struct pair_t *hash_table_lookup(struct hash_table_t *table, char *key) {
  struct pair_t *find;
  unsigned long  hash;

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

void getnc(char *dst, size_t n) {
  int  i;
  char c;

  assert(dst);

  while ((c = getchar()) == '\n') {
  }

  for (i = 0; i < n; i++) {
    *(dst++) = c;
    c        = getchar();
  }

  *dst = '\0';
}

int gettokn(char *src) {
  int r = 1;

  assert(src);

  while ((src = strchr(src, ' '))) {
    r++;
    src++;
  }

  return r;
}

int main() {
  struct hash_table_t *table;
  struct pair_t       *pair;
  char                *buf, *tok;
  int                  a, l, n;

  scanf("%d %d", &a, &l);

  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  getnc(buf, l);

  n = gettokn(buf);

  table = hash_table_init(n);

  tok = strtok(buf, " ");
  while (tok) {
    pair = hash_table_lookup(table, tok);

    /* Value is already present */
    if (pair) {
      pair->value += 1;
    }
    /* Pair is new, create and insert */
    else {
      pair = pair_init(tok, 1);
      hash_table_insert(table, pair);
    }
    tok = strtok(NULL, " ");
  }
  free(buf);

  scanf("%d", &l);
  buf = calloc(l + 1, sizeof(char));
  assert(buf);
  getnc(buf, l);

  n = gettokn(buf);

  tok = strtok(buf, " ");
  while (tok) {
    pair = hash_table_lookup(table, tok);

    if (!pair) {
      printf("%d ", 0);
    } else {
      printf("%d ", pair->value);
    }
    tok = strtok(NULL, " ");
  }

  free(buf);
  printf("\n");

  hash_table_free(table);
}
