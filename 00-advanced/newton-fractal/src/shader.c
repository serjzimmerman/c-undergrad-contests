#include "shader.h"

#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <fcntl.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "logger.h"

char *shader_root_folder = NULL;

GLuint shader_compile(char *name, GLenum type) {
  if (!shader_root_folder) {
    logger("Shader folder path not set\n");
    return 0;
  }

  struct stat st;
  char fullpath[PATH_MAX];

  strncpy(fullpath, shader_root_folder, PATH_MAX);
  strcat(fullpath, name);

  int res = stat(fullpath, &st);

  if (res == -1) {
    logger("Unable to open shader source file\n");
    return 0;
  }

  char *source = calloc(st.st_size + 1, sizeof(char));

  if (!source) {
    logger("Unable to allocate memory for source string\n");
    return 0;
  }

  int fd = open(fullpath, O_RDONLY);
  read(fd, source, st.st_size);

  GLuint shader = glCreateShader(type);
  if (shader == 0) {
    logger("Unable to create shader\n");
    return 0;
  }

  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  char fail_log[512];

  glGetShaderiv(shader, GL_COMPILE_STATUS, &res);
  if (!res) {
    glGetShaderInfoLog(shader, 512, NULL, fail_log);
    logger("Failed to compile shader %s, error: %s", fullpath, fail_log);
  }

  return shader;
}

GLuint shader_program_link(GLuint vertex, GLuint fragment) {
  GLuint program = glCreateProgram();

  if (program == 0) {
    logger("Unable to create program\n");
    return 0;
  }

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);

  glLinkProgram(program);

  int success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);

  char fail_log[512];

  if (!success) {
    glGetProgramInfoLog(program, 512, NULL, fail_log);
    logger("Error linking shader program: %s\n", fail_log);
    return 0;
  }

  return program;
}

int shader_set_folder(const char *folder) {
  assert(folder);

  DIR *dir = opendir(folder);
  if (!dir) {
    logger("Cannot access shader folder: %s", folder);
    return -1;
  }

  closedir(dir);
  shader_root_folder = folder;

  return 0;
}