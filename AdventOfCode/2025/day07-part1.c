#include <stdio.h>

#define DIM 142 + 1

int main() {
  FILE *f = fopen("input07.txt", "r");

  int count = 0;
  char input[DIM][DIM];

  // Copy input into matrix
  for (int i = 0; i < DIM; i++) {
    fgets(input[i], DIM, f);
  }

  // The strategy is to simulate the manifold
  for (int i = 0; i < DIM - 1; i++) {
    for (int j = 0; j < DIM; j++) {
      if (input[i][j] == 'S' || input[i][j] == '|') {
        if (input[i + 1][j] == '^') {
          input[i + 1][j - 1] = '|';
          input[i + 1][j + 1] = '|';
          count++;
        } else {
          input[i + 1][j] = '|';
        }
      }
    }
  }

  printf("Total splits: %d\n", count);
  fclose(f);
}