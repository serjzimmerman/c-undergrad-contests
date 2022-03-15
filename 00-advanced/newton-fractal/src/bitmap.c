#include "bitmap.h"
#include <assert.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/errno.h>
#include <unistd.h>

struct bitmap_s {
  unsigned width, height;
  color_rgb_t color;
  point_t point;
  color_rgb_t *array;
};

bitmap_t *bitmap_create(unsigned width, unsigned height) {
  bitmap_t *bitmap = calloc(1, sizeof(bitmap_t));

  if (!bitmap) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  bitmap->width = width;
  bitmap->height = height;

  bitmap->array = calloc(width * height, sizeof(color_rgb_t));
  if (!bitmap->array) {
    fprintf(stderr, "Unable to allocate memory\n");
    exit(EXIT_FAILURE);
  }

  return bitmap;
}

#define BMP_HEADER_SIZE 54
#define BYTES_PER_PIXEL 3

#pragma pack(1)
struct bitmap_header_s {
  union {
    struct {
      /* First part of the bmp header */
      unsigned char bitmap_name[2]; /* Always equal to "BM" in ASCII = 0x4D42 */
      uint32_t file_size;           /* Total file size in bytes */
      unsigned char reserved[4];    /* Must be equal to 0 */
      uint32_t header_size;         /* Total header size = 54 */
      /* Second part of the bmp header */
      uint32_t second_size; /* Size of the second header part = 40 */
      uint32_t width;
      uint32_t height;
      uint16_t color_planes; /* Must be equal to 1 */
      uint16_t bits_per_pixel;
      uint32_t compression;
      uint32_t image_size; /* Can be equal to dummy 0 */
      uint32_t dummy_bits[4];
    };
    unsigned char array[BMP_HEADER_SIZE];
  };
};

struct bitmap_header_s header_create(bitmap_t *image) {
  assert(image);

  struct bitmap_header_s header = {0};

  header.bitmap_name[0] = 'B';
  header.bitmap_name[1] = 'M';

  header.header_size = BMP_HEADER_SIZE;
  header.second_size = 40;

  header.height = image->height;
  header.width = image->width;
  header.file_size = BMP_HEADER_SIZE + header.height * header.width * BYTES_PER_PIXEL;

  header.bits_per_pixel = CHAR_BIT * BYTES_PER_PIXEL;
  header.color_planes = 1;

  return header;
}

void bitmap_save(bitmap_t *image, const char *save_path) {
  assert(image);

  int fd = open(save_path, O_CREAT | O_RDWR, 0666);

  if (fd == -1) {
    fprintf(stderr, "Unable to open file %s\n", save_path);
    exit(EXIT_FAILURE);
  }

  struct bitmap_header_s header = header_create(image);

  int res = write(fd, &header.array, BMP_HEADER_SIZE);
  if (res == -1) {
    fprintf(stderr, "Unable to write to file %s\nError: %d\n", save_path, errno);
    exit(EXIT_FAILURE);
  }

  res = write(fd, image->array, image->height * image->width * BYTES_PER_PIXEL);
  if (res == -1) {
    fprintf(stderr, "Unable to write to file %s\nError: %d\n", save_path, errno);
    exit(EXIT_FAILURE);
  }

  close(fd);
}

void bitmap_put_pixel(bitmap_t *image, unsigned x, unsigned y, color_rgb_t color) {
  assert(image);
  assert(x < image->width && y < image->height);

  image->array[x + y * image->width] = color;
}

void bitmap_fill(bitmap_t *image, color_rgb_t color) {
  assert(image);

  for (unsigned i = 0; i < image->height * image->width; ++i) {
    image->array[i] = color;
  }
}

void bitmap_free(bitmap_t *image) {
  free(image->array);
  free(image);
}

unsigned bitmap_get_width(bitmap_t *image) {
  assert(image);
  return image->width;
}

unsigned bitmap_get_height(bitmap_t *image) {
  assert(image);
  return image->height;
}