#ifndef UTIL_H
#define UTIL_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <sys/time.h>

static long get_millis() {
  struct timeval te;
  gettimeofday(&te, NULL);
  return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

#endif