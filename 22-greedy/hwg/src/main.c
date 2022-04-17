#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdio.h>

#define DEBUG

#if defined(DEBUG)
#define WARNING(format, ...) MESSAGE("Warning", format, ##__VA_ARGS__);
#define ERROR(format, ...)                                                                                                                 \
  do {                                                                                                                                     \
    MESSAGE("Fatal error", format, ##__VA_ARGS__);                                                                                         \
    exit(EXIT_FAILURE);                                                                                                                    \
  } while (0)

#else
#define WARNING(format, ...)                                                                                                               \
  do {                                                                                                                                     \
  } while (0)
#define ERROR(format, ...)                                                                                                                 \
  do {                                                                                                                                     \
    MESSAGE(format, ##__VA_ARGS__);                                                                                                        \
    exit(EXIT_FAILURE);                                                                                                                    \
  } while (0)
#endif

#if defined(DEBUG)
#define MESSAGE(errtype, format, ...)                                                                                                      \
  do {                                                                                                                                     \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__, ##__VA_ARGS__);      \
  } while (0)

#else
#define MESSAGE(errtype, format, ...)                                                                                                      \
  do {                                                                                                                                     \
    fprintf(stderr, format, ##__VA_ARGS__);                                                                                                \
  } while (0)

#endif
#define ASSERTION(condition)                                                                                                               \
  if (!(condition)) {                                                                                                                      \
    do {                                                                                                                                   \
      MESSAGE("Assertion (" #condition ") failed", "");                                                                                    \
      exit(EXIT_FAILURE);                                                                                                                  \
    } while (0);                                                                                                                           \
  }

#if defined(DEBUG)
#define CALLOC_CHECKED(pointer, count, size)                                                                                               \
  do {                                                                                                                                     \
    (pointer) = calloc(count, size);                                                                                                       \
    if (!(pointer)) {                                                                                                                      \
      ERROR("Memory exhausted, cannot allocate enough memory of total size: %lu\n", (unsigned long)(count) * (size));                      \
    }                                                                                                                                      \
  } while (0)
#else
#define CALLOC_CHECKED(pointer, count, size)                                                                                               \
  do {                                                                                                                                     \
    (pointer) = calloc(count, size);                                                                                                       \
    if (!(pointer)) {                                                                                                                      \
      ERROR("Memory exhausted\n");                                                                                                         \
    }                                                                                                                                      \
  } while (0)
#endif

typedef struct vertex_node_s {
  struct vertex_node_s *next;
  /* Index of the vertex that this node is adjacent to */
  size_t index;
  /* Weight of the edge connecting this->next */
  int weight;
} vertex_node_t;

vertex_node_t *vertex_node_init(size_t index, int weight) {
  vertex_node_t *result;
  CALLOC_CHECKED(result, 1, sizeof(vertex_node_t));
  result->index  = index;
  result->weight = weight;
  return result;
}

void vertex_node_free(vertex_node_t *node) {
  free(node);
}

typedef struct {
  vertex_node_t *first, *last;

  size_t len;
} vertex_list_t;

vertex_list_t *vertex_list_init() {
  vertex_list_t *list;
  CALLOC_CHECKED(list, 1, sizeof(vertex_list_t));
  return list;
}

void vertex_list_push_front(vertex_list_t *list, vertex_node_t *node) {
  ASSERTION(list);
  ASSERTION(node);

  if (list->len++ == 0) {
    list->first = list->last = node;
    return;
  }

  node->next  = list->first;
  list->first = node;
}

void vertex_list_pop_front(vertex_list_t *list) {
  vertex_node_t *node;

  ASSERTION(list);
  ASSERTION(list->len);

  if (list->len-- == 1) {
    free(list->first);
    list->first = list->last = NULL;
    return;
  }

  node = list->first->next;
  free(list->first);
  list->first = node;
}

void vertex_list_free(vertex_list_t *list) {
  vertex_node_t *curr, *next;

  ASSERTION(list);

  curr = list->first;
  while (curr) {
    next = curr->next;
    vertex_node_free(curr);
    curr = next;
  }

  free(list);
}

typedef struct {
  size_t          num_vert, num_edge;
  vertex_list_t **adj_list;
} ud_graph_t;

ud_graph_t *ud_graph_init(size_t size) {
  ud_graph_t *graph;
  size_t      i;

  ASSERTION(size);
  CALLOC_CHECKED(graph, 1, sizeof(ud_graph_t));

  CALLOC_CHECKED(graph->adj_list, size, sizeof(vertex_list_t *));
  graph->num_vert = size;
  for (i = 0; i < size; ++i) {
    graph->adj_list[i] = vertex_list_init();
  }

  return graph;
}

void ud_graph_free(ud_graph_t *graph) {
  size_t i;

  ASSERTION(graph);
  for (i = 0; i < graph->num_vert; ++i) {
    vertex_list_free(graph->adj_list[i]);
  }

  free(graph->adj_list);
  free(graph);
}

void ud_graph_add_edge(ud_graph_t *graph, size_t first, size_t second, int weight) {
  vertex_node_t *node1, *node2;

  ASSERTION(graph);
  ASSERTION(first != second);

  node1 = vertex_node_init(second, weight);
  node2 = vertex_node_init(first, weight);

  vertex_list_push_front(graph->adj_list[first], node1);
  vertex_list_push_front(graph->adj_list[second], node2);

  graph->num_edge++;
}

int ud_graph_has_cycle_impl_(ud_graph_t *graph, size_t vert, char *visited, size_t parent) {
  vertex_node_t *current;
  size_t         i;

  ASSERTION(graph);
  ASSERTION(visited);

  visited[vert] = 1;

  current = graph->adj_list[vert]->first;
  for (i = 0; i < graph->adj_list[vert]->len; ++i) {
    if (!visited[current->index]) {
      if (ud_graph_has_cycle_impl_(graph, current->index, visited, vert)) {
        return 1;
      }
    } else if (current->index != parent) {
      return 1;
    }
    current = current->next;
  }

  return 0;
}

int ud_graph_has_cycle(ud_graph_t *graph, char *visited_) {
  char  *visited;
  int    result = 0;
  size_t i;

  ASSERTION(graph);

  if (!visited_) {
    CALLOC_CHECKED(visited, graph->num_vert, sizeof(char));
  } else {
    visited = visited_;
  }

  for (i = 0; i < graph->num_vert; ++i) {
    if (!visited[i] && ud_graph_has_cycle_impl_(graph, i, visited, ULONG_MAX)) {
      result = 1;
      goto ud_graph_has_cycle_exit;
    }
  }

ud_graph_has_cycle_exit:

  if (!visited_) {
    free(visited);
  }
  return result;
}

typedef struct {
  size_t first, second;
  int    weight;
} ud_edge_t;

int ud_edge_cmp(const void *a, const void *b) {
  ud_edge_t *first, *second;

  first  = (ud_edge_t *)a;
  second = (ud_edge_t *)b;

  return (first->weight - second->weight);
}

ud_edge_t *ud_edge_arr(ud_graph_t *graph) {
  vertex_node_t *curr;
  ud_edge_t     *array;
  size_t         i, j;

  ASSERTION(graph);
  CALLOC_CHECKED(array, graph->num_edge, sizeof(ud_edge_t));

  for (i = 0, j = 0; i < graph->num_vert; ++i) {
    curr = graph->adj_list[i]->first;
    while (curr) {
      if (i < curr->index) {
        array[j].first  = i;
        array[j].second = curr->index;
        array[j].weight = curr->weight;
        j++;
      }
      curr = curr->next;
    }
  }

  qsort(array, graph->num_edge, sizeof(ud_edge_t), ud_edge_cmp);
  return array;
}

int spanning_tree_min(ud_graph_t *graph) {
  ud_graph_t *copy;
  ud_edge_t  *array;
  size_t      i;
  char       *visited;
  int         result = 0;

  CALLOC_CHECKED(visited, graph->num_vert, sizeof(char));
  array = ud_edge_arr(graph);
  copy  = ud_graph_init(graph->num_vert);

  for (i = 0; i < graph->num_edge; ++i) {
    ud_graph_add_edge(copy, array[i].first, array[i].second, array[i].weight);

    if (ud_graph_has_cycle(copy, visited)) {
      vertex_list_pop_front(copy->adj_list[array[i].first]);
      vertex_list_pop_front(copy->adj_list[array[i].second]);
    } else {
      result += array[i].weight;
    }

    memset(visited, 0, graph->num_vert);
  }

  free(visited);
  free(array);
  ud_graph_free(copy);

  return result;
}

int main() {
  size_t      n, first, second;
  ud_graph_t *graph;
  int         res, weight;

  res = scanf("%lu", &n);
  if (res != 1) {
    ERROR("Invalid input\n");
  }

  graph = ud_graph_init(n);
  do {
    res = scanf("%lu %lu %d", &first, &second, &weight);
    if (res != 3) {
      break;
    }
    ud_graph_add_edge(graph, first, second, weight);
  } while (res == 3);

  printf("%d\n", spanning_tree_min(graph));
  ud_graph_free(graph);
}
