#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#define ULL_BIT 63

struct Point {
  float x;
  float y;
};

int in_region(struct Point p) {
  if (p.y > -2 && p.x > -4 && p.y > (1 / (p.x + 4) - 3)) {
    return 1;
  }
  return 0;
}
