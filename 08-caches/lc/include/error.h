#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>

#define UNUSED_PARAMETER(expr) (void)(expr);

#if defined(NDEBUG) && (NDEBUG == 1)
#undef DEBUG
#else
#define DEBUG
#endif

#if defined(DEBUG)
#define MESSAGE(errtype, format, ...)                                                                                  \
  do {                                                                                                                 \
    fprintf(stderr, "%s: at line %d of %s in function %s\n" format, errtype, __LINE__, __FILE__, __PRETTY_FUNCTION__,  \
            ##__VA_ARGS__);                                                                                            \
  } while (0)

#else
#define MESSAGE(format, ...)                                                                                           \
  do {                                                                                                                 \
    fprintf(stderr, format, ##__VA_ARGS__);                                                                            \
  } while (0)
#endif

#if defined(DEBUG)
#define WARNING(format, ...) MESSAGE("Warning", format, ##__VA_ARGS__);
#define ERROR(format, ...)                                                                                             \
  do {                                                                                                                 \
    MESSAGE("Fatal error", format, ##__VA_ARGS__);                                                                     \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)

#else
#define WARNING(format, ...)                                                                                           \
  do {                                                                                                                 \
  } while (0)
#define ERROR(format, ...)                                                                                             \
  do {                                                                                                                 \
    MESSAGE(format, ##__VA_ARGS__);                                                                                    \
    exit(EXIT_FAILURE);                                                                                                \
  } while (0)
#endif

#ifdef DEBUG
#define ASSERTION(condition)                                                                                           \
  if (!(condition)) {                                                                                                  \
    MESSAGE("Assertion (" #condition ") failed", "");                                                                  \
    exit(EXIT_FAILURE);                                                                                                \
  }
#else
#define ASSERTION(condition)                                                                                           \
  do {                                                                                                                 \
    UNUSED_PARAMETER(condition);                                                                                       \
  } while (0)
#endif

#endif