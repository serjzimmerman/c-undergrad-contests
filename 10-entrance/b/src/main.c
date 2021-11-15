#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float time_stairs(int n, int m, int t) {
  return abs(n - m) * t;
}

float time_elev(int n, int m, int k, float ta, float tb) {
  return (abs(n - k) * ta + 2 * tb + abs(n - m) * ta);
}

int main() {
  int N = 0, M = 0, K = 0;
  float Ta = 0, Tb = 0, Tc = 0;

  scanf("%d %d %d %f %f %f", &N, &M, &K, &Ta, &Tb, &Tc);

  printf("%s\n",
         (time_stairs(N, M, Tc) < time_elev(N, M, K, Ta, Tb) ? "stairs\n"
                                                             : "elevator\n"));

  return 0;
}
