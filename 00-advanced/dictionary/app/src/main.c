#include "counter.h"
#include "hashtable.h"
#include "util.h"

#undef NDEBUG

#include <assert.h>
#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#define PRINT_DEBUG_INFO

struct cmd_args_t {
  char *input_path;
  char *output_path;
  int to_lower;
} cmd_args_default = {NULL, NULL, 0};

const char *const usage_string = "Usage: wordcounter [-l] [-i <path>] [-o <path>]\n";

int handle_input(int argc, char **argv, struct cmd_args_t *args) {
  int c, p = 0;

  *args = cmd_args_default;

  while ((c = getopt(argc, argv, "li:o:h")) != -1) {
    switch (c) {
    case 'i':
      args->input_path = optarg;
      p += 2;
      break;
    case 'o':
      args->output_path = optarg;
      p += 2;
      break;
    case 'l':
      args->to_lower = 1;
      p += 1;
      break;
    case 'h':
      fprintf(stderr, usage_string);
      return -1;
    case '?':
      if (optopt == 'i' || optopt == 'o') {
        fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      }
      return -1;
    default:
      exit(EXIT_FAILURE);
    }
  }

  if (argc > p + 1 || args->input_path == NULL) {
    fprintf(stderr, "Invalid arguments. Run -h for help.\n");
    return -1;
  }

  return 0;
}

char *get_buf_from_file(struct cmd_args_t args, int *fd, size_t *len) {
  struct stat st;
  char *buf;
  int fdi;

  fdi = open(args.input_path, O_RDONLY);
  if (fdi == -1) {
    fprintf(stderr, "Could not open input file %s\n", args.input_path);
    exit(EXIT_FAILURE);
  }
  if (fstat(fdi, &st) == -1) {
    fprintf(stderr, "Could not open input file %s\n", args.input_path);
    exit(EXIT_FAILURE);
  }
  buf = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fdi, 0);
  *fd = fdi;
  *len = st.st_size;

  return buf;
}

void application_loop_read_file(struct cmd_args_t args) {
  struct counter_t *counter;
  struct sl_list_t *list;
  char *buf, *tok;
  int fdi, fdo;
  size_t len;
  FILE *fo;

  buf = get_buf_from_file(args, &fdi, &len);

  counter = counter_init(NULL);

  tok = strtok(buf, " \n\t\".,[]()");
  while (tok) {
    counter_item_add(counter, (args.to_lower ? stolower(tok) : tok));
    tok = strtok(NULL, " \n\t\".,[]()");
  }

#ifdef PRINT_DEBUG_INFO
  fprintf(stderr, "\nCollisions: \t%ld\nInserts: \t%ld\nSize: \t\t%ld\n",
          hash_table_get_collisions(counter_get_hashtable(counter)),
          hash_table_get_inserts(counter_get_hashtable(counter)), hash_table_get_size(counter_get_hashtable(counter)));
#endif

  fo = stdout;

  if (args.output_path != NULL) {
    fdo = open(args.output_path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fdi == -1) {
      fprintf(stderr, "Could not open output file %s\n", args.input_path);
      exit(EXIT_FAILURE);
    }
    fo = fdopen(fdo, "w");
  }

  list = hash_table_get_linked_list(counter_get_hashtable(counter), 1);
  sl_list_merge_sort(list, pair_cmp);
  sl_list_print(list, fo);

  counter_free(counter, 0);

  munmap(buf, len);
  close(fdi);

  if (args.output_path != NULL) {
    fclose(fo);
  }

  sl_list_free(list, pair_free);
}

int main(int argc, char **argv) {
  struct cmd_args_t args;

  if (handle_input(argc, argv, &args)) {
    exit(EXIT_FAILURE);
  }

  application_loop_read_file(args);
}