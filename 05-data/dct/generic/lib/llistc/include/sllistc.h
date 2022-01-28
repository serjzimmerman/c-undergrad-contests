#ifndef SLLISTC_H
#define SLLISTC_H

#include <stdarg.h>

/* Singly linked list implementation */

/* List node */
struct sl_node_t;
/* List header */
struct sl_list_t;

struct sl_node_t *sl_node_init();
void *sl_node_get_data(struct sl_node_t *const node);
void sl_node_set_data(struct sl_node_t *const node, void *const data);
void sl_node_free(struct sl_node_t *const node);

struct sl_list_t *sl_list_init();
int sl_list_is_empty(const struct sl_list_t *const list);
struct sl_list_t *sl_list_push(struct sl_list_t *const list, struct sl_node_t *const node);
struct sl_list_t *sl_list_append(struct sl_list_t *const list, struct sl_node_t *const node);
void sl_list_free(struct sl_list_t *const list, void (*datafree)(void *data));
void sl_list_iterate_over_nodes(struct sl_list_t *const list,
                                void (*callback)(struct sl_node_t *node, struct sl_list_t *list,
                                                 va_list argp),
                                ...);
void sl_list_insert_after(struct sl_list_t *const list, struct sl_node_t *const prev, struct sl_node_t *node);
void sl_list_merge_sort(struct sl_list_t *list, int (*cmp_func)(void *a, void *b));
struct sl_node_t *sl_node_get_n_next(struct sl_node_t *const node, int n);
struct sl_node_t *sl_list_get_head(struct sl_list_t *const list);
void sl_node_set_next(struct sl_node_t *const node, struct sl_node_t *const next);
void sl_list_remove_node(struct sl_list_t *const list, struct sl_node_t *prev, struct sl_node_t *node);

#endif