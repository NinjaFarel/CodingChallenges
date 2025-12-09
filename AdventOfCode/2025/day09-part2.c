#include <stdbool.h>
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
  bool valid;

  // Read input
  for (int i = 0; i < N_TILES; i++) {
    fscanf(f, "%d,%d", &red_tiles[i].x, &red_tiles[i].y);
  }

  // Calculate all possible rectangles
  for (int i = 0; i < N_TILES - 1; i++) {
    for (int j = i + 1; j < N_TILES; j++) {

      int a_lef =
          (red_tiles[i].x < red_tiles[j].x) ? red_tiles[i].x : red_tiles[j].x;
      int a_rig =
          (red_tiles[i].x > red_tiles[j].x) ? red_tiles[i].x : red_tiles[j].x;
      int a_bot =
          (red_tiles[i].y < red_tiles[j].y) ? red_tiles[i].y : red_tiles[j].y;
      int a_top =
          (red_tiles[i].y > red_tiles[j].y) ? red_tiles[i].y : red_tiles[j].y;

      // Check if it exits
      valid = true;
      for (int k = 0; k < N_TILES; k++) {
        int k2 = (k + 1) % N_TILES;

        int b_lef = (red_tiles[k].x < red_tiles[k2].x) ? red_tiles[k].x
                                                       : red_tiles[k2].x;
        int b_rig = (red_tiles[k].x > red_tiles[k2].x) ? red_tiles[k].x
                                                       : red_tiles[k2].x;
        int b_bot = (red_tiles[k].y < red_tiles[k2].y) ? red_tiles[k].y
                                                       : red_tiles[k2].y;
        int b_top = (red_tiles[k].y > red_tiles[k2].y) ? red_tiles[k].y
                                                       : red_tiles[k2].y;

        bool x_overlap = (b_rig > a_lef && b_lef < a_rig);
        bool y_overlap = (b_top > a_bot && b_bot < a_top);

        if (x_overlap && y_overlap) {
          valid = false;
          break;
        }
      }

      if (!valid)
        continue;

      temp = (long long)(a_rig - a_lef + 1) * (long long)(a_top - a_bot + 1);
      if (count < temp) {
        count = temp;
      }
    }
  }

  printf("Largest possible area: %lld\n", count);
  fclose(f);
}