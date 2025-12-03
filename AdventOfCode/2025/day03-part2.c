#include <stdio.h>

#define BANKS 200
#define BATTERIES 100
#define ACTIVATED 12

int main() {
  FILE *f = fopen("input03.txt", "r");

  int bank[BATTERIES];
  long long count = 0, max_digits[ACTIVATED], max_is[ACTIVATED + 1], joltage;
  max_is[0] = -1;

  for (int b = 0; b < BANKS; b++) {
    // Input
    for (int i = 0; i < BATTERIES; i++) {
      fscanf(f, "%1d", &bank[i]);
    }

    // Max digits
    for (int d = 0; d < ACTIVATED; d++) {
      max_is[d + 1] = max_is[d] + 1;
      max_digits[d] = bank[max_is[d] + 1];
      for (int i = max_is[d] + 2; i < BATTERIES - ACTIVATED + d + 1; i++) {
        if (max_digits[d] < bank[i]) {
          max_is[d + 1] = i;
          max_digits[d] = bank[i];
        }
      }
    }

    // Calculate joultage
    joltage = 0;
    for (int d = 0; d < ACTIVATED; d++) {
      joltage += max_digits[d];
      joltage *= 10;
    }
    joltage /= 10;

    // nprintf("%d. %lld\n", b + 1, joltage);
    count += joltage;
  }

  printf("Sum of max joltage: %lld\n", count);
  fclose(f);
}