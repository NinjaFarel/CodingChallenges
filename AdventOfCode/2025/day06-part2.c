#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_NUMBERS 2000
#define ROWS 4

int main() {
  FILE *f = fopen("input06.txt", "r");

  char lines[ROWS + 1][MAX_NUMBERS * 5];
  long long result, count = 0;
  char column[ROWS], operation;
  int numbers[ROWS] = {0}, n_length, line_length = 0;
  bool all_spaces = true;

  // Read the lines
  for (int r = 0; r < ROWS + 1; r++) {
    fgets(lines[r], sizeof(lines[0]), f);
  }
  line_length = strlen(lines[0]);
  for (int r = 0; r < ROWS; r++) {
    lines[r][line_length - 1] = ' '; // Needed to trigger the last problem
  }

  // Analize the line one column at a time for the win
  for (int i = 0; i < line_length; i++) {
    // If this is a new problem, read the operation
    if (all_spaces)
      sscanf(lines[ROWS] + i, " %c", &operation);

    // Read the numbers
    for (int r = 0; r < ROWS; r++) {
      sscanf(lines[r] + i, "%c", &column[r]);
    }

    all_spaces = true;
    for (int r = 0; r < ROWS; r++) {
      if (column[r] == ' ')
        column[r] = '0';
      else
        all_spaces = false;
    }

    if (all_spaces) {
      // Line has ended, calculate
      if (operation == '+') {
        result = 0;
        for (int c = 0; c < n_length; c++) {
          result += numbers[c];
        }
      } else {
        result = 1;
        for (int c = 0; c < n_length; c++) {
          result *= numbers[c];
        }
      }
      count += result;

      // Reset for next problem
      n_length = 0;
    } else {
      // Convert the numbers
      numbers[n_length] = 0;
      for (int r = 0; r < ROWS; r++) {
        if (column[r] != '0' || numbers[n_length] == 0) {
          // Digit shifting + ASCII conversion
          numbers[n_length] = (numbers[n_length] * 10) + (column[r] - 48);
        }
      }
      n_length++;
    }
  }

  printf("Grand total: %lld\n", count);
  fclose(f);
}