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

#include "hashtable.h"
#include "logger.h"
#include "shader.h"

char *shader_root_folder = NULL;

struct shader_s {
  GLuint id;
  const char *name;
};

shader_t *shader_init() {
  shader_t *shader = calloc(1, sizeof(shader_t));
  return shader;
}

void shader_free(shader_t *shader) {
  assert(shader);

  /* Valid shader handles are nonzero unsigned int values */
  assert(shader->id);

  glDeleteShader(shader->id);
  free(shader);
}

#define SHADER_LOG_SIZE 512
shader_t *shader_compile(const char *name, char *folder, GLenum type) {
  assert(folder);
  assert(name);

  if (!folder) {
    logger("Shader folder path not set\n");
    return 0;
  }

  struct stat st;
  char fullpath[PATH_MAX];

  strncpy(fullpath, folder, PATH_MAX);
  strncat(fullpath, name, PATH_MAX - strlen(folder));

  int res = stat(fullpath, &st);

  if (res == -1) {
    logger("Unable to open shader source file\n");
    return NULL;
  }

  char *const source = calloc(st.st_size + 1, sizeof(char));

  if (!source) {
    logger("Unable to allocate memory for source string\n");
    return NULL;
  }

  int fd = open(fullpath, O_RDONLY);
  read(fd, source, st.st_size);

  GLuint id = glCreateShader(type);
  if (id == 0) {
    logger("Unable to create shader\n");
    return NULL;
  }

  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  char fail_log[SHADER_LOG_SIZE];

  glGetShaderiv(id, GL_COMPILE_STATUS, &res);
  if (!res) {
    glGetShaderInfoLog(id, SHADER_LOG_SIZE, NULL, fail_log);
    logger("Failed to compile shader %s, error: %s", fullpath, fail_log);
  }

  shader_t *shader = calloc(1, sizeof(shader_t));
  if (!shader) {
    return NULL;
  }

  shader->id = id;
  shader->name = name;

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

#define DEFAULT_MAX_SHADERS 32
struct shader_manager_s {
  struct hash_table_t *table;
  char *folder_path;
  unsigned max_shaders;
};

/* Hash the shader name to use in shader manager hashtable */
unsigned long shader_hash(const void *shader) {
  assert(shader);
  return hash_djb2(((shader_t *)shader)->name);
}

int shader_cmp(void *shader1, void *shader2) {
  assert(shader1);
  assert(shader2);

  const char *name1 = ((shader_t *)shader1)->name;
  const char *name2 = ((shader_t *)shader2)->name;

  return strcmp(name1, name2);
}

int shader_manager_inits(shader_manager_t *manager, const char *folder) {
  assert(manager);
  assert(folder);

  DIR *dir = opendir(folder);
  if (!dir) {
    logger("Cannot access shader folder: %s", folder);
    return -1;
  }

  closedir(dir);

  manager->folder_path = folder;
  manager->max_shaders = DEFAULT_MAX_SHADERS;

  manager->table = hash_table_init(manager->max_shaders, shader_hash, shader_cmp, (void (*)(void *))shader_free);
  if (!manager->table) {
    logger("Unable to create shader manager");
    return -1;
  }

  return 0;
}

shader_manager_t *shader_manager_init(const char *folder) {
  shader_manager_t *manager;

  assert(folder);

  manager = calloc(1, sizeof(shader_manager_t));
  if (!manager) {
    return NULL;
  }

  int res = shader_manager_inits(manager, folder);
  if (res) {
    free(manager);
    return NULL;
  }

  return manager;
}

/* Maybe inmplement caching algorithm for compiled shaders */
GLuint shader_manager_get(shader_manager_t *manager, const char *name, GLenum type) {
  assert(manager);
  assert(name);

  shader_t *shader, *lookup, key = {0, name};
  lookup = hash_table_lookup(manager->table, &key);

  if (lookup != NULL) {
    return lookup->id;
  }

  shader = shader_compile(name, manager->folder_path, type);

  if (!shader) {
    return 0;
  }

  int res = hash_table_insert(&manager->table, shader);

  if (res) {
    shader_free(shader);
    return 0;
  }

  return shader->id;
}