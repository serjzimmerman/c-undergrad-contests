#version 330 core

uniform vec2 iResolution;
in vec3      iPos;

void main() {
  gl_Position = vec4(iPos.x, iPos.y, iPos.z, 1.0);
}