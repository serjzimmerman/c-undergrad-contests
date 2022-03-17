#ifndef LOGGER_H
#define LOGGER_H

#include <stdarg.h>
#include <stdio.h>

#ifdef VERBOSE
static void logger(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
}
#else
static void logger(char *fmt, ...) {
  return;
}
#endif
#endif