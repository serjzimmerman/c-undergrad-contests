#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "hashtable.h"

struct shader_s;
typedef struct shader_s shader_t;

struct shader_manager_s;
typedef struct shader_manager_s shader_manager_t;

int               shader_set_folder(const char *folder);
shader_t         *shader_compile(const char *name, char *folder, GLenum type);
GLuint            shader_program_link(GLuint vertex, GLuint fragment);
shader_manager_t *shader_manager_init(const char *folder);
GLuint            shader_manager_get(shader_manager_t *manager, const char *name, GLenum type);

#ifdef __cplusplus
}
#endif

#endif