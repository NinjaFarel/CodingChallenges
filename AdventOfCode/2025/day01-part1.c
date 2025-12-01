#include <stdio.h>

int main() {
  FILE *f = fopen("input01.txt", "r");

  char dir;
  int len, pos = 50, sign, count = 0;

  while (fscanf(f, "%c %d\n", &dir, &len) != EOF) {
    sign = dir == 'R' ? 1 : -1;
    pos = (pos + sign * len) % 100;
    pos = pos < 0 ? pos % 100 : pos;
    count += pos == 0;
    // printf("dir=%c, len=%d, pos=%d, count=%d\n", dir, len, pos, count);
  }
  printf("Password: %d\n", count);

  fclose(f);
}