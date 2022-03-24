#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>

#if defined(DEBUG)

#define warning(format, ...) message("warning", format, ##__VA_ARGS__);
#define error(format, ...)                                                                                             \
  do {                                                                                                                 \
    message("fatal error", format, ##__VA_ARGS__);                                                                     \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)

#else
#define warning(format, ...)                                                                                           \
  do {                                                                                                                 \
  } while (0)
#define error(format, ...)                                                                                             \
  do {                                                                                                                 \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)
#endif

#define message(errtype, format, ...)                                                                                  \
  do {                                                                                                                 \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__,  \
            ##__VA_ARGS__);                                                                                            \
  } while (0)

#define assertion(condition)                                                                                           \
  if (!(condition)) {                                                                                                  \
    do {                                                                                                               \
      message("Assertion (" #condition ") failed", "");                                                                \
      exit(EXIT_FAILURE);                                                                                              \
    } while (0);                                                                                                       \
  }
#endif
