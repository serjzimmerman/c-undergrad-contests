#include "bitmap.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "logger.h"
#include "shader.h"
#include "util.h"

#include <cglm/cglm.h>

#define ZOOMSPEED 1e-4f

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

struct {
  int width, height;
} resolution = {1920, 1080};

#define MAX_ROOTS 64
struct {
  vec2  center;
  float zoom;

  shader_manager_t *manager;

  GLuint program;
  GLuint VBO, VAO, EBO;
  long   initial_ms;
} newton_drawer;

float vertices[] = {
    1.0f,  1.0f,  0.0f, // top right
    1.0f,  -1.0f, 0.0f, // bottom right
    -1.0f, -1.0f, 0.0f, // bottom left
    -1.0f, 1.0f,  0.0f  // top left
};
unsigned int indices[] = {
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  resolution.height = height;
  resolution.width  = width;
}

GLFWwindow *window_init() {
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

  GLFWwindow *window = glfwCreateWindow(resolution.width, resolution.height, "Newton Fractal", NULL, NULL);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (window == NULL) {
    logger("Unable to create window\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwMakeContextCurrent(window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK) {
    logger("Failed to initialize GLEW\n");
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  return window;
}

void setup_newton() {
  newton_drawer.manager = shader_manager_init("shaders/");
  if (!newton_drawer.manager) {
    logger("Unable to initialize shader manager");
    exit(EXIT_FAILURE);
  }

  GLuint vertex_shader = shader_manager_get(newton_drawer.manager, "vertex.vs", GL_VERTEX_SHADER);
  if (vertex_shader == 0) {
    logger("Error compiling vertex shader\n");
    exit(EXIT_FAILURE);
  }

  GLuint frag_shader = shader_manager_get(newton_drawer.manager, "frag.fs", GL_FRAGMENT_SHADER);
  if (frag_shader == 0) {
    logger("Error compiling fragment shader\n");
    exit(EXIT_FAILURE);
  }

  newton_drawer.program = shader_program_link(vertex_shader, frag_shader);
  if (newton_drawer.program == 0) {
    exit(EXIT_FAILURE);
  }

  newton_drawer.zoom = 1.0f;
  glm_vec2_zero(newton_drawer.center);
  newton_drawer.initial_ms = get_millis();

  glGenVertexArrays(1, &newton_drawer.VAO);
  glGenBuffers(1, &newton_drawer.VBO);
  glGenBuffers(1, &newton_drawer.EBO);

  glBindVertexArray(newton_drawer.VAO);

  glBindBuffer(GL_ARRAY_BUFFER, newton_drawer.VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newton_drawer.EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);
}

void draw_newton() {
  mat3 mat;

  float current_ms = get_millis() - newton_drawer.initial_ms;

  newton_drawer.zoom = exp((-1) * current_ms * ZOOMSPEED);
  glUseProgram(newton_drawer.program);
  /* Calculate translation matrix */
  glm_mat3_identity(mat);
  glm_translate2d(mat, (vec2){0, 0});
  glm_scale2d(mat, (vec2){(float)resolution.width / (float)resolution.height * newton_drawer.zoom, newton_drawer.zoom});

  GLint iResolutionPosition = glGetUniformLocation(newton_drawer.program, "iResolution");
  glUniform2f(iResolutionPosition, resolution.width, resolution.height);

  GLint iTranslatePos = glGetUniformLocation(newton_drawer.program, "iTranslate");
  glUniformMatrix3fv(iTranslatePos, 1, GL_FALSE, &mat[0][0]);

  glBindVertexArray(newton_drawer.VAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void exit_newton() {
  glDeleteVertexArrays(1, &newton_drawer.VAO);
  glDeleteBuffers(1, &newton_drawer.VBO);
  glDeleteBuffers(1, &newton_drawer.EBO);
  glDeleteProgram(newton_drawer.program);

  shader_manager_free(newton_drawer.manager);
}

int main() {
  GLFWwindow *window = window_init();

  setup_newton();

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    draw_newton();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  exit_newton();

  glfwTerminate();
  return 0;
}