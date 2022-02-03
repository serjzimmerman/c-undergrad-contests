#include <assert.h>
#include <getopt.h>
#include <linux/limits.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "counter.h"

struct cmd_args_t {
  char *input_path;
  char *output_path;
} cmd_args_default = {"", "out.txt", 0, 0};

/* TODO: implement app */