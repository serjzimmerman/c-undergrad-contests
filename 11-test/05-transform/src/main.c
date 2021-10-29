#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ULL_BIT 63

struct Point {
  int x;
  int y;
};
struct Rect {
  struct Point lt;
  struct Point rb;
};

struct Rect transform(struct Rect r) {
  struct Rect result = {};

  result.lt = r.rb;

  result.rb.x = result.lt.x + (r.rb.x - r.lt.x);
  result.rb.y = result.lt.y - (r.lt.y - r.rb.y);

  return result;
}

int main() {
  struct Rect rect, a = {{-1, 1}, {4, -2}};
  rect = transform(a);
  printf("%d %d %d %d\n", a.lt.x, a.lt.y, a.rb.x, a.rb.y);
  printf("%d %d %d %d\n", rect.lt.x, rect.lt.y, rect.rb.x, rect.rb.y);
  return 0;
}
