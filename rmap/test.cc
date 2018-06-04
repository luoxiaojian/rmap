#include <stdio.h>

#include "rmap.h"

int main(int argc, char **argv) {
  rmap<int, int> m;
  printf("----\n");

  m[5] = 0;
  m[4] = 1;
  m[3] = 2;
  m[2] = 3;
  m[1] = 4;
  m[0] = 5;

  for (int i = 0; i < 6; i++) {
    printf("%d\n", m[i]);
  }

  return 0;
}
