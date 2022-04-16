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
  size_t          num_vert;
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

void ud_graph_add_edge(ud_graph_t *graph, size_t first, size_t second) {
  vertex_node_t *node1, *node2;

  ASSERTION(graph);
  ASSERTION(first != second);

  node1 = vertex_node_init(second, 0);
  node2 = vertex_node_init(first, 0);

  vertex_list_push_front(graph->adj_list[first], node1);
  vertex_list_push_front(graph->adj_list[second], node2);
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

int ud_graph_has_cycle(ud_graph_t *graph) {
  char  *visited;
  int    result = 0;
  size_t i;

  ASSERTION(graph);
  CALLOC_CHECKED(visited, graph->num_vert, sizeof(char));

  for (i = 0; i < graph->num_vert; ++i) {
    if (!visited[i] && ud_graph_has_cycle_impl_(graph, i, visited, ULONG_MAX)) {
      result = 1;
      goto ud_graph_has_cycle_exit;
    }
  }

ud_graph_has_cycle_exit:
  free(visited);
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
    ud_graph_add_edge(graph, first, second);
  } while (res == 3);

  printf("%d\n", ud_graph_has_cycle(graph));
}
