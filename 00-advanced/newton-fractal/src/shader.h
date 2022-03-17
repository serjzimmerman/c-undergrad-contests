#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

int shader_set_folder(const char *folder);
GLuint shader_compile(char *filepath, GLenum type);
GLuint shader_program_link(GLuint vertex, GLuint fragment);

#endif