#include <stdio.h>

#define MAX_NUMBERS 2000
#define ROWS 4

int main() {
  FILE *f = fopen("input06.txt", "r");

  long long result, count = 0;
  int numbers[MAX_NUMBERS][ROWS], total_ops = -1;
  char line[MAX_NUMBERS * 5], c;
  char operations[MAX_NUMBERS] = {'\0'};

  // Read numbers
  for (int r = 0; r < ROWS; r++) {
    fgets(line, MAX_NUMBERS * 5, f);

    int i = 0, tot_read = 0, read;
    while (sscanf(line + tot_read, "%d%n", &numbers[i][r], &read) != EOF) {
      i++;
      tot_read += read;
    }
  }

  // Read operations
  while (fscanf(f, " %c", &operations[++total_ops]) != EOF) {
  }

  // Calculate
  for (int i = 0; i < total_ops; i++) {
    if (operations[i] == '+') {
      result = 0;
      for (int j = 0; j < ROWS; j++) {
        result += numbers[i][j];
      }
    } else {
      result = 1;
      for (int j = 0; j < ROWS; j++) {
        result *= numbers[i][j];
      }
    }

    count += result;
  }

  printf("Grand total: %lld\n", count);
  fclose(f);
}