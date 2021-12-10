#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct Point {
  int x;
  int y;
};

struct Rect {
  struct Point lt; // left top
  struct Point rb; // right bottom
};

struct Rect *transform(const struct Rect *src) {
  struct Rect *rect = calloc(1, sizeof(struct Rect));

  rect->rb = src->lt;

  struct Point p = src->lt;

  p.x = p.x * 2 - src->rb.x;
  p.y = p.y * 2 - src->rb.y;

  rect->lt = p;

  return rect;
}

int main() {
  struct Rect a = {{-1, 1}, {4, -2}};
  struct Rect *rect;

  rect = transform(&a);

  printf("%d %d %d %d\n", a.lt.x, a.lt.y, a.rb.x, a.rb.y);
  printf("%d %d %d %d\n", rect->lt.x, rect->lt.y, rect->rb.x, rect->rb.y);

  free(rect);
  return 0;
}
