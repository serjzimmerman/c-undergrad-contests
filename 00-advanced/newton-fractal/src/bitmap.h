#ifndef BITMAP_H
#define BITMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <assert.h>

typedef struct {
  unsigned char b, g, r;
} color_rgb_t;

typedef struct {
  unsigned h, s, v;
} color_hsv_t;

#define COLOR_BLUE                                                                                                     \
  (color_rgb_t) {                                                                                                      \
    255, 0, 0                                                                                                          \
  }

#define COLOR_RED                                                                                                      \
  (color_rgb_t) {                                                                                                      \
    255, 0, 0                                                                                                          \
  }

#define COLOR_BLACK                                                                                                    \
  (color_rgb_t) {                                                                                                      \
    0, 0, 0                                                                                                            \
  }

#define COLOR_WHITE                                                                                                    \
  (color_rgb_t) {                                                                                                      \
    255, 255, 255                                                                                                      \
  }

static inline unsigned rgb_to_hex(color_rgb_t rgb) {
  return (rgb.r << 16) | (rgb.g << 8) | (rgb.b);
}

static inline color_rgb_t hex_to_rgb(unsigned hex) {
  color_rgb_t rgb = {0};

  assert(hex & 0xffffff == hex);
  rgb.r = hex >> 16;
  rgb.g = (hex >> 8) & 0xff;
  rgb.b = hex & 0xff;

  return rgb;
}

static inline color_rgb_t rgb_from_colors(unsigned char r, unsigned char g, unsigned char b) {
  color_rgb_t rgb;

  rgb.r = r;
  rgb.g = g;
  rgb.b = b;

  return rgb;
}

/* https://ru.wikipedia.org/wiki/HSV_(цветовая_модель) */
static inline color_rgb_t hsv_to_rgb(color_hsv_t hsv) {
  unsigned    v_min, v_inc, v_dec, a;
  color_rgb_t rgb = {0};

  unsigned char hi = hsv.h / 60;

  /* Calculate rgb values in percents */
  v_min = (100 - hsv.s) * hsv.v / 100;
  a     = (hsv.v - v_min) * (hsv.h % 60) / 60;
  v_inc = v_min + a;
  v_dec = hsv.v - a;

  /* Convert to [0, 255] range */
  hsv.v = (hsv.v * 255) / 100;
  v_inc = (v_inc * 255) / 100;
  v_dec = (v_dec * 255) / 100;
  v_min = (v_min * 255) / 100;

  switch (hi) {
  case 0:
    rgb = rgb_from_colors(hsv.v, v_inc, v_min);
    break;
  case 1:
    rgb = rgb_from_colors(v_dec, hsv.v, v_min);
    break;
  case 2:
    rgb = rgb_from_colors(v_min, hsv.v, v_inc);
    break;
  case 3:
    rgb = rgb_from_colors(v_min, v_dec, hsv.v);
    break;
  case 4:
    rgb = rgb_from_colors(v_inc, v_min, hsv.v);
    break;
  case 5:
    rgb = rgb_from_colors(hsv.v, v_min, v_dec);
    break;
  }

  return rgb;
}

typedef struct {
  unsigned x, y;
} point_t;

struct bitmap_s;
typedef struct bitmap_s bitmap_t;

bitmap_t *bitmap_create(unsigned width, unsigned height);
void      bitmap_save(bitmap_t *image, const char *save_path);
void      bitmap_put_pixel(bitmap_t *image, unsigned x, unsigned y, color_rgb_t color);
void      bitmap_free(bitmap_t *image);
unsigned  bitmap_get_width(bitmap_t *image);
unsigned  bitmap_get_height(bitmap_t *image);
void      bitmap_fill(bitmap_t *image, color_rgb_t color);

#ifdef __cplusplus
}
#endif

#endif