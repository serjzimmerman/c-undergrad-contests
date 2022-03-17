#include "bitmap.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <float.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <time.h>

#include "logger.h"
#include "shader.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

long long GetMillis() {
  struct timeval te;

  gettimeofday(&te, NULL);

  return te.tv_sec * 1000LL + te.tv_usec / 1000;
}

int main() {
  /* Initialize GLFW */
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    exit(EXIT_FAILURE);
  }

  shader_set_folder("shaders/");

  GLuint vertex_shader = shader_compile("vertex.vs", GL_VERTEX_SHADER);

  if (vertex_shader == 0) {
    logger("Error compiling vertex shader\n");
    exit(EXIT_FAILURE);
  }
  GLuint frag_shader = shader_compile("frag.fs", GL_FRAGMENT_SHADER);
  if (frag_shader == 0) {
    logger("Error compiling fragment shader\n");
    exit(EXIT_FAILURE);
  }

  GLuint shaderProgram = shader_program_link(vertex_shader, frag_shader);
  if (shaderProgram == 0) {
    exit(EXIT_FAILURE);
  }

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      1.0f,  1.0f,  0.0f, // top right
      1.0f,  -1.0f, 0.0f, // bottom right
      -1.0f, -1.0f, 0.0f, // bottom left
      -1.0f, 1.0f,  0.0f  // top left
  };
  unsigned int indices[] = {
      // note that we start from 0!
      0, 1, 3, // first Triangle
      1, 2, 3  // second Triangle
  };

  unsigned int dimensions[] = {
      1920,
      1080,
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);
  // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex
  // buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // remember: do NOT unbind the EBO while a VAO is active as the bound element buffer object IS stored in the VAO; keep
  // the EBO bound.
  // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens.
  // Modifying other VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when
  // it's not directly necessary.
  glBindVertexArray(0);

  // uncomment this call to draw in wireframe polygons.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  // render loop
  // -----------
  float zoom = 0.250f; //

  long initial_ms = GetMillis();

  while (!glfwWindowShouldClose(window)) {
    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw our first triangle
    glUseProgram(shaderProgram);

    GLint iResolutionPosition = glGetUniformLocation(shaderProgram, "iResolution");
    glUniform2f(iResolutionPosition, 1920.f, 1080.f);
    GLint iCenterPos = glGetUniformLocation(shaderProgram, "iCenter");
    glUniform2f(iCenterPos, -0.16f, 1.0405f);
    GLint iZoomPos = glGetUniformLocation(shaderProgram, "iZoom");
    glUniform1f(iZoomPos, zoom);

    float delta_ms = (float)(GetMillis() - initial_ms);
    if (delta_ms > 2000.0f) {
      zoom = 0.250f * exp((delta_ms - 2000.0f) / 5000);
    }

    GLint iTimePos = glGetUniformLocation(shaderProgram, "iTime");
    glUniform1f(iTimePos, delta_ms);

    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do
                            // so to keep things a bit more organized
                            // glDrawArrays(GL_TRIANGLES, 0, 6);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  // make sure the viewport matches the new window dimensions; note that width and
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}