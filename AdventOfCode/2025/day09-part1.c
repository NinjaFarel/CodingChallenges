#include <stdio.h>
#include <stdlib.h>

#define N_TILES 496
#define N_RECTS (N_TILES * (N_TILES - 1)) / 2

typedef struct Tile {
  int x;
  int y;
} Tile;

int main() {
  FILE *f = fopen("input09.txt", "r");

  Tile red_tiles[N_TILES];
  long long temp, count = 0;

  // Read input
  for (int i = 0; i < N_TILES; i++) {
    fscanf(f, "%d,%d", &red_tiles[i].x, &red_tiles[i].y);
  }

  // Calculate all possible rectangles
  for (int i = 0; i < N_TILES - 1; i++) {
    for (int j = i + 1; j < N_TILES; j++) {
      temp =
          (long long)((long long)abs((red_tiles[i].x - red_tiles[j].x)) + 1) *
          ((long long)abs((red_tiles[i].y - red_tiles[j].y)) + 1);
      if (count < temp) {
        count = temp;
      }
    }
  }

  printf("Largest possible area: %lld\n", count);
  fclose(f);
}