#include <stdio.h>

#define DIM 142

int main() {
  FILE *f = fopen("input07.txt", "r");

  long long count = 0;
  long input[DIM][DIM], temp;

  // Copy input into matrix
  for (int i = 0; i < DIM; i++) {
    for (int j = 0; j < DIM; j++) {
      temp = fgetc(f);
      switch (temp) {
      case '^':
        input[i][j] = -1;
        break;
      case 'S':
        input[i][j] = 1;
        break;
      default:
        input[i][j] = 0;
        break;
      }
    }
  }

  // The strategy is to simulate the manifold... but with numbers!
  for (int i = 0; i < DIM - 1; i++) {
    for (int j = 0; j < DIM; j++) {
      if (input[i][j] > 0) {
        if (input[i + 1][j] == -1) {
          input[i + 1][j - 1] += input[i][j];
          input[i + 1][j + 1] += input[i][j];
        } else {
          input[i + 1][j] += input[i][j];
        }
      }
    }
  }

  // Count how many quantum tachyons arrive to the last line
  for (int i = 0; i < DIM - 1; i++) {
    count += input[DIM - 1][i];
  }

  printf("Total timelines: %lld\n", count);
  fclose(f);
}