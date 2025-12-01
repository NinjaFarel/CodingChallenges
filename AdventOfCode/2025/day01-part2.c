#include <stdio.h>
#include <stdlib.h>

int main() {
  FILE *f = fopen("input01.txt", "r");

  char dir;
  int len, pos = 50, sign, count = 0, prev;

  while (fscanf(f, "%c %d\n", &dir, &len) != EOF) {
    sign = dir == 'R' ? 1 : -1;
    count += len / 100;
    len = len % 100;
    prev = pos;
    pos += sign * len;

    if ((pos < 0 || pos > 100) && prev != 0) {
      count++;
    }

    if (pos < 0)
      pos += 100;
    pos = abs(pos) % 100;

    if (pos == 0)
      count++;

    // printf("dir=%c, len=%d, pos=%d, count=%d\n", dir, len, pos, count);
  }
  printf("Password: %d\n", count);

  fclose(f);
}