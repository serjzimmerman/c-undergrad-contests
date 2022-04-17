#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdarg.h>
#include <stdio.h>

#include "error.h"
#include "memutil.h"
#include "udgraph.h"

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
