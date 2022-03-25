#version 330 core

uniform vec2  iResolution;
uniform float iTime;

uniform mat3 iTranslate;

in vec4 gl_FragCoord;

out vec4 fragColor;

vec2 complex_mul(vec2 a, vec2 b) {
  return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

vec2 complex_add(vec2 a, vec2 b) {
  return vec2(a.x + b.x, a.y + b.y);
}

vec2 complex_div(vec2 n, vec2 d) {
  return vec2((n.x * d.x + n.y * d.y), (n.y * d.x - n.x * d.y)) / dot(d, d);
}

/* Hardcore function f(z) = z^3 - 1 = 0 */

vec2 compute_value(vec2 pos) {
  return complex_mul(complex_mul(pos, pos), pos) - vec2(1.0, 0.0);
}

vec2 compute_derivative(vec2 pos) {
  return 3 * complex_mul(pos, pos);
}

const vec2 roots[3] = vec2[3](vec2(1.0, 0.0), vec2(-0.5, -0.866), vec2(-0.5, 0.866));

void main() {
  vec3 ndf = vec3(gl_FragCoord.x / iResolution.x - 0.5, gl_FragCoord.y / iResolution.y - 0.5, 1.0);

  vec2 pos = (iTranslate * ndf).xy;
  vec3 col = vec3(1.0, 1.0, 1.0);

  for (int i = 0; i < 64; ++i) {
    pos -= complex_div(compute_value(pos), compute_derivative(pos));
  }

  int   closest = 0;
  float dist    = dot(pos - roots[0], pos - roots[0]);

  float temp;
  for (int i = 0; i < 3; ++i) {
    if ((temp = dot(pos - roots[i], pos - roots[i])) < dist) {
      closest = i;
      dist    = temp;
    }
  }

  switch (closest) {
  case 0:
    fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    break;
  case 1:
    fragColor = vec4(0.0, 1.0, 0.0, 1.0);
    break;
  case 2:
    fragColor = vec4(0.0, 0.0, 1.0, 1.0);
    break;
  }
}
