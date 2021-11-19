#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct min_path {
  int min;
  int init;
};

void set_min(struct min_path *min, int val) {
  if (min->init) {
    if (val < min->min) {
      min->min = val;
    }
  } else {
    min->min = val;
    min->init = 1;
  }
}

void reset_min(struct min_path *min, int val) {
  memset(min, 0, sizeof(struct min_path));
}

int int_cmp(const int *x, const int *y) {
  return *x - *y;
}

int money_change(int money, int *coins, int *buf, int nc, int bufsz) {
  int i, j, c;
  struct min_path min = {0};
  
  buf[0] = 0;

  for (i = 1; i <= money; i++) {
    min.init = 0;
    min.min = 0;

    for (j = 0; j < nc && (c = i - coins[j]) >= 0; j++) {
      if (buf[c] != -1) {
        set_min(&min, buf[c]);
      }
    }
    
    if (min.init) {
      buf[i] = min.min + 1;
    } else {
      buf[i] = -1;
    }
  }

  return buf[money];
}

int main() {
  int money, nc, *coins, *buf, i;

  scanf("%d %d", &money, &nc);
  
  coins = (int *)calloc(nc, sizeof(int));
  
  for (i = 0; i < nc; i++) {
    scanf("%d", &coins[i]);
  }
  
  qsort(coins, nc, sizeof(int), int_cmp);
  
  buf = (int *)calloc(money + 1, sizeof(int));

  printf("%d\n", money_change(money, coins, buf, nc, money + 1));
  
  /* for (i = 0; i <= money; i++) {
    printf("%d, ", buf[i]);
  } */

  free(coins);
  free(buf); 
  
  return 0;
}
