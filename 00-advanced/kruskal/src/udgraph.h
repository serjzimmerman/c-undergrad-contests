#ifndef UD_GRAPH_H
#define UD_GRAPH_H

#include <ctype.h>
#include <stddef.h>

struct vertex_node_s;
typedef struct vertex_node_s vertex_node_t;

struct vertex_list_s;
typedef struct vertex_list_s vertex_list_t;

struct ud_graph_s;
typedef struct ud_graph_s ud_graph_t;

typedef struct {
  size_t first, second;
  int    weight;
} ud_edge_t;

ud_graph_t *ud_graph_init(size_t size);
int         spanning_tree_min(ud_graph_t *graph);
void        ud_graph_add_edge(ud_graph_t *graph, size_t first, size_t second, int weight);
void        ud_graph_free(ud_graph_t *graph);

#endif