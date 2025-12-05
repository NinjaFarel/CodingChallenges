#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define N_RANGES 192

int compare(const void *r1p, const void *r2p) {
  const long long *r1 = (const long long *)r1p;
  const long long *r2 = (const long long *)r2p;

  return (r1[0] > r2[0]) - (r1[0] < r2[0]);
}

int main() {
  FILE *f = fopen("input05.txt", "r");

  long long ranges[N_RANGES][2];
  unsigned long long count = 0;
  int good = 0;

  // Read ranges
  for (int i = 0; i < N_RANGES; i++) {
    fscanf(f, "%lld-%lld", &ranges[i][0], &ranges[i][1]);
  }

  // Merge ranges
  qsort(ranges, N_RANGES, sizeof(ranges[0]), compare);
  for (int i = 1; i < N_RANGES; i++) {
    if (ranges[good][1] >= ranges[i][0]) {
      if (ranges[i][1] > ranges[good][1]) {
        ranges[good][1] = ranges[i][1];
      }
    } else {
      good++;
      ranges[good][0] = ranges[i][0];
      ranges[good][1] = ranges[i][1];
    }
  }

  // Count ids
  for (int i = 0; i <= good; i++) {
    count += ranges[i][1] - ranges[i][0] + 1;
  }

  printf("Fresh ingredients: %lld\n", count);
  fclose(f);
}