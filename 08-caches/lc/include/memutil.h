#ifndef MEMUTIL_H
#define MEMUTIL_H

#include "error.h"

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

#endif