#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *f = fopen("input02.txt", "r");

  long start, end, count = 0, len;
  char str[20], substr[20];

  while (fscanf(f, "%ld-%ld,", &start, &end) != EOF) {
    // printf("start=%ld, end=%ld\n", start, end);
    for (long i = start; i <= end; i++) {
      sprintf(str, "%ld", i);
      len = strlen(str);
      if (len % 2 == 1)
        continue;
      strncpy(substr, str + (len / 2), (len / 2));
      substr[(len / 2)] = '\0';
      if (strncmp(str, substr, len / 2) == 0) {
        // printf("%s - %s (%d)\n", str, substr, len);
        count += i;
      }
    }
  }
  printf("Count: %ld\n", count);

  fclose(f);
}