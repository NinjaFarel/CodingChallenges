#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *f = fopen("input02.txt", "r");

  long long start, end, count = 0;
  char str[20], substr[20];
  int str_len, substr_occ;
  bool invalid;

  while (fscanf(f, "%ld-%ld,", &start, &end) != EOF) {
    // printf("start=%ld, end=%ld\n", start, end);
    for (long i = start; i <= end; i++) {
      sprintf(str, "%ld", i);
      str_len = strlen(str);

      // For every divisor...
      for (int d = 1; d <= str_len / 2; d++) {
        if (str_len % d != 0)
          continue;

        // ...create the substr...
        strncpy(substr, str, d);
        substr[d] = '\0';
        substr_occ = str_len / d;

        // ...and check if it repeats
        invalid = true;
        for (int k = 1; k < substr_occ; k++)
          if (strncmp(str + (d * k), substr, d) != 0) {
            invalid = false;
            break;
          }
        if (invalid) {
          count += i;
          break;
        }
      }
    }
  }
  printf("Count: %lld\n", count);

  fclose(f);
}