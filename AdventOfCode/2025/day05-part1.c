#include <stdbool.h>
#include <stdio.h>

#define N_RANGES 192
#define N_IDS 1000

int main() {
  FILE *f = fopen("input05.txt", "r");

  int count = 0;
  long long ranges[N_RANGES][2], id;
  bool fresh;

  // Read ranges
  for (int i = 0; i < N_RANGES; i++) {
    fscanf(f, "%lld-%lld", &ranges[i][0], &ranges[i][1]);
  }
  fscanf(f, "\n", NULL);

  // Process IDs
  for (int i = 0; i < N_IDS; i++) {
    fscanf(f, "%ld", &id);
    fresh = false;
    for (int j = 0; j < N_RANGES; j++) {
      if (id >= ranges[j][0] && id <= ranges[j][1]) {
        fresh = true;
        break;
      }
    }
    if (fresh)
      count++;
  }

  printf("Fresh ingredients: %d\n", count);
  fclose(f);
}