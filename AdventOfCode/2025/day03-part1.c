#include <stdio.h>

#define BANKS 200
#define BATTERIES 100

int main() {
  FILE *f = fopen("input03.txt", "r");

  int bank[BATTERIES], count = 0, max_tens, i_max_tens, max_units;

  for (int b = 0; b < BANKS; b++) {
    // Input
    for (int i = 0; i < BATTERIES; i++) {
      fscanf(f, "%1d", &bank[i]);
    }

    // Max tens
    i_max_tens = 0;
    max_tens = bank[i_max_tens];
    for (int i = 1; i < BATTERIES - 1; i++) {
      if (max_tens < bank[i]) {
        i_max_tens = i;
        max_tens = bank[i_max_tens];
      }
    }

    // Max units
    max_units = bank[i_max_tens + 1];
    for (int i = i_max_tens + 1; i < BATTERIES; i++) {
      if (max_units < bank[i])
        max_units = bank[i];
    }

    // printf("%d. %d%d\n", b+1, max_tens, max_units);
    count += max_tens * 10 + max_units;
  }

  printf("Sum of max joltage: %d\n", count);
  fclose(f);
}