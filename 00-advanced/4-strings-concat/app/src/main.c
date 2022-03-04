#include "counter.h"
#include "dspair.h"
#include "hashtable.h"
#include "util.h"

#undef NDEBUG

#include <assert.h>
#include <ctype.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define PRINT_DEBUG_INFO

struct cmd_args_t {
  char *input_path;
} cmd_args_default = {NULL};

const char *const usage_string = "Usage: fstrings [-i <path>]\n";

int handle_input(int argc, char **argv, struct cmd_args_t *args) {
  int c, p = 0;

  *args = cmd_args_default;

  while ((c = getopt(argc, argv, "o:i:h")) != -1) {
    switch (c) {
    case 'i':
      args->input_path = optarg;
      p += 2;
      break;
    case 'h':
      fprintf(stderr, usage_string);
      return -1;
    case '?':
      if (optopt == 'i') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      }
      return -1;
    default:
      exit(EXIT_FAILURE);
    }
  }

  if (argc > p + 1) {
    fprintf(stderr, "Invalid arguments. Run -h for help.\n");
    return -1;
  }

  return 0;
}

#define DEFAULT_BUF_SIZE 1024

struct buf_t {
  char *buf;
  size_t size, pos;
};

struct buf_t *buf_init(size_t size) {
  struct buf_t *buf;

  buf = calloc(1, sizeof(struct buf_t));
  if (!buf) {
    return NULL;
  }

  buf->size = size;
  buf->buf = calloc(buf->size, sizeof(char));

  if (!buf) {
    free(buf);
    return NULL;
  }

  return buf;
}

void buf_free(struct buf_t *buf) {
  free(buf->buf);
  free(buf);
}

int buf_resize(struct buf_t *buf, size_t size) {
  char *temp;

  temp = realloc(buf->buf, size);
  if (!temp) {
    return 1;
  }

  buf->buf = temp;
  buf->size = size;

  return 0;
}

int buf_append(struct buf_t *buf, const char c) {
  if (buf->pos == buf->size) {
    int res = buf_resize(buf, 2 * buf->size);
    if (res) {
      return res;
    }
  }

  buf->buf[buf->pos++] = c;

  return 0;
}

/* Close your eyes, goto madness ahead */
int buf_readnstr(struct buf_t *buf, size_t n) {
  size_t i;
  char c;

  for (i = 0; i < n; i++) {
    while (isspace(c = getchar()))
      ;

    if (c == EOF)
      goto readnstr_exit;

    for (;;) {
      if (isspace(c))
        break;

      if (buf_append(buf, c))
        goto readnstr_error;

      c = getchar();
    }

    if (i != n - 1) {
      if (buf_append(buf, ' '))
        goto readnstr_error;
    }
  }

readnstr_exit:

  if (buf_append(buf, '\0'))
    return -1;

  return i;

readnstr_error:
  return -1;
}

char **get_array_of_tokens(char *buf, size_t n) {
  char **array;
  size_t i = 0;

  array = calloc(n, sizeof(char *));
  if (!array) {
    return NULL;
  }

  char *tok = strtok(buf, " ");
  while (tok) {
    array[i++] = tok;
    tok = strtok(NULL, " ");
  }

  return array;
}

struct hash_table_t *hash_pairs_from_array(char **array, size_t n) {
  struct hash_table_t *table;
  dspair_t *pair;

  table = hash_table_init(10 * n * (n - 1), dspair_hash, dspair_cmp, dspair_free);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < n; j++) {
      if (i == j)
        continue;

      pair = dspair_init(array[i], array[j]);
      if (!pair) {
        hash_table_free(table);
        return NULL;
      }

      hash_table_insert(&table, pair);
    }
  }

  return table;
}

#define VERBOSE

void application_loop_read_stdin() {
  int n, res;
  res = scanf("%d", &n);

  if (res != 1) {
    fprintf(stderr, "Invalid input");
    exit(EXIT_FAILURE);
  }

  struct buf_t *buf = buf_init(256);
  res = buf_readnstr(buf, n);

  if (res != n) {
    fprintf(stderr, (res == -1 ? "Internal error" : "Invalid input"));
    exit(EXIT_FAILURE);
  }

  char **array = get_array_of_tokens(buf->buf, n);

  if (!array) {
    fprintf(stderr, "Internal error");
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < n; i++) {
    printf("%s\n", array[i]);
  }

  clock_t begin = clock();

  struct hash_table_t *table = hash_pairs_from_array(array, n);

  clock_t end = clock();

#ifdef VERBOSE
  fprintf(stderr, "Constructing hash table took: %.3lf seconds\n", (double)(end - begin) / CLOCKS_PER_SEC);
#endif
  // sl_list_print(hash_table_get_linked_list(table, 0), stdout);

  hash_table_free(table);
  free(array);
  buf_free(buf);

#ifdef VERBOSE
#endif
}

int main(int argc, char **argv) {
  struct cmd_args_t args;

  if (handle_input(argc, argv, &args)) {
    exit(EXIT_FAILURE);
  }

  application_loop_read_stdin();
}