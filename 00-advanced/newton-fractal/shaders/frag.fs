#version 330 core

uniform vec2 iResolution;
uniform vec2 iCenter;
uniform float iTime;
uniform float iZoom;

in vec4 gl_FragCoord;

out vec4 fragColor;

vec2 complex_mul(vec2 a, vec2 b) {
  return vec2(a.x * b.x - a.y * b.y, a.x * b.y + a.y * b.x);
}

vec2 complex_add(vec2 a, vec2 b) {
  return vec2(a.x + b.x, a.y + b.y);
}

void main() {
  vec2 uv = vec2(gl_FragCoord.x / iResolution.x, gl_FragCoord.y / iResolution.y);
  float aspect_ratio = iResolution.x / iResolution.y;

  vec2 ndf = uv - vec2(0.5, 0.5);

  vec2 pos = (vec2(ndf.x * aspect_ratio, ndf.y) / iZoom + iCenter);
  vec3 col = vec3(1.0, 1.0, 1.0);

  vec2 initial_pos = pos;

  vec2 julia_offset = vec2(cos(iTime / 1000) / 2, sin(iTime / 1000) / 2);

  for (int i = 0; i < 512; ++i) {
    pos = complex_mul(pos, pos) + initial_pos;

    if (dot(pos, pos) > 4.0) {
      col = vec3(float(i) / 512.0, float(i) / 512.0, float(i) / 512.0);
      break;
    }
  }

  fragColor = vec4(vec3(1.0, 1.0, 1.0) - col, 1.0);
}
