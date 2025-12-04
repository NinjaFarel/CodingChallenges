#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROWS 135
#define COLS 135

int main() {
  FILE *f = fopen("input04.txt", "r");

  // Adding two empty rows and cols for easier checking logic
  bool wall[ROWS + 2][COLS + 2] = {{0}};
  bool wall2[ROWS + 2][COLS + 2] = {{0}};

  char wall3[ROWS + 2][COLS + 2] = {{0}};

  char input;
  int count = 0, n_adj;
  bool removed;

  // Input
  for (int i = 1; i < ROWS + 1; i++) {
    for (int j = 1; j < COLS + 1; j++) {
      fscanf(f, "%c", &input);
      wall2[i][j] = input == '@' ? 1 : 0;
    }
    fscanf(f, "\n", NULL);
  }

  // Counting
  do {
    removed = false;
    memcpy(wall, wall2, (ROWS + 2) * (COLS + 2));

    for (int i = 1; i < ROWS + 1; i++) {
      for (int j = 1; j < COLS + 1; j++) {
        if (!wall[i][j])
          continue;

        n_adj = 0;

        // A bit ugly
        if (wall[i - 1][j - 1])
          n_adj++;
        if (wall[i - 1][j])
          n_adj++;
        if (wall[i - 1][j + 1])
          n_adj++;
        if (wall[i][j - 1])
          n_adj++;
        if (wall[i][j + 1])
          n_adj++;
        if (wall[i + 1][j - 1])
          n_adj++;
        if (wall[i + 1][j])
          n_adj++;
        if (wall[i + 1][j + 1])
          n_adj++;

        if (n_adj < 4) {
          count++;
          wall2[i][j] = 0;
          removed = true;
        }
      }
    }
  } while (removed);

  printf("Accessible rolls: %d\n", count);
  fclose(f);
}