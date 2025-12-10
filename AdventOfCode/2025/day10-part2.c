#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INF 1000000000000000LL

// Structure to store a combination of button presses
typedef struct {
  int *joltage;
  int presses;
} Combo;

// Cache for memoization
typedef struct CacheEntry {
  int *target;
  long long value;
  struct CacheEntry *next;
} CacheEntry;

#define CACHE_SIZE 10007
CacheEntry *cache[CACHE_SIZE];
int g_n_reqs;

// Simple hash function for the target joltage array
unsigned int hash_target(int target[]) {
  unsigned int h = 0;
  for (int i = 0; i < g_n_reqs; i++) {
    h = h * 31 + target[i];
  }
  return h % CACHE_SIZE;
}

// Get value from cache
long long get_cache(int target[]) {
  unsigned int h = hash_target(target);
  CacheEntry *e = cache[h];
  while (e) {
    bool match = true;
    for (int i = 0; i < g_n_reqs; i++) {
      if (e->target[i] != target[i]) {
        match = false;
        break;
      }
    }
    if (match)
      return e->value;
    e = e->next;
  }
  return -1;
}

// Set value in cache
void set_cache(int target[], long long value) {
  unsigned int h = hash_target(target);
  CacheEntry *e = malloc(sizeof(CacheEntry));
  e->target = malloc(sizeof(int) * g_n_reqs);
  memcpy(e->target, target, sizeof(int) * g_n_reqs);
  e->value = value;
  e->next = cache[h];
  cache[h] = e;
}

// Clear cache between machines
void clear_cache() {
  for (int i = 0; i < CACHE_SIZE; i++) {
    CacheEntry *e = cache[i];
    while (e) {
      CacheEntry *next = e->next;
      free(e->target);
      free(e);
      e = next;
    }
    cache[i] = NULL;
  }
}

// Recursive function to find the minimum presses using bifurcation
long long count_presses(int target[], Combo **combos_by_pattern,
                        int n_combos_by_pattern[]) {
  bool all_zero = true;
  for (int i = 0; i < g_n_reqs; i++) {
    if (target[i] < 0)
      return INF;
    if (target[i] > 0)
      all_zero = false;
  }
  if (all_zero)
    return 0;

  long long cached = get_cache(target);
  if (cached != -1)
    return cached;

  // Calculate the pattern (parity of each requirement)
  int pattern = 0;
  for (int i = 0; i < g_n_reqs; i++) {
    if (target[i] % 2 != 0)
      pattern |= (1 << i);
  }

  long long min_total = INF;
  // Try all combinations that match the parity pattern
  for (int i = 0; i < n_combos_by_pattern[pattern]; i++) {
    int next_target[g_n_reqs];
    for (int j = 0; j < g_n_reqs; j++) {
      next_target[j] =
          (target[j] - combos_by_pattern[pattern][i].joltage[j]) / 2;
    }
    long long res =
        count_presses(next_target, combos_by_pattern, n_combos_by_pattern);
    if (res != INF) {
      long long total = combos_by_pattern[pattern][i].presses + 2 * res;
      if (total < min_total)
        min_total = total;
    }
  }

  set_cache(target, min_total);
  return min_total;
}

int main() {
  FILE *f = fopen("input10.txt", "r");
  long long count = 0;
  char buff[1000];

  while (fgets(buff, sizeof(buff), f) != NULL) {
    // Read input - Joltage requirements
    char *find_start = strchr(buff, '{');
    char *find_end = strchr(find_start, '}');
    int n_reqs = 0;
    for (char *p = find_start; p < find_end; p++) {
      if (*p == ',' || *p == '{')
        n_reqs++;
    }
    g_n_reqs = n_reqs;

    int joltage_goal[n_reqs];
    FILE *req_file = fmemopen(find_start + 1, find_end - find_start - 1, "r");
    for (int i = 0; i < n_reqs; i++) {
      fscanf(req_file, "%d", &joltage_goal[i]);
      getc(req_file);
    }
    fclose(req_file);

    // Read input - Button wiring
    int n_buttons = 0;
    for (int i = 0; buff[i] != '\0'; i++) {
      if (buff[i] == '(')
        n_buttons++;
    }

    bool buttons[n_buttons][n_reqs];
    memset(buttons, 0, sizeof(buttons));
    find_start = buff;
    for (int i = 0; i < n_buttons; i++) {
      find_start = strchr(find_start + 1, '(');
      find_end = strchr(find_start, ')');
      FILE *button_file =
          fmemopen(find_start + 1, find_end - find_start - 1, "r");

      int n = 0;
      while (fscanf(button_file, "%d", &n) != EOF) {
        if (n < n_reqs)
          buttons[i][n] = true;
        getc(button_file);
      }
      fclose(button_file);
    }

    // Fill patterns and combos (combinations of buttons mod 2)
    int n_patterns = 1 << n_reqs;
    Combo *combos_by_pattern[n_patterns];
    int n_combos_by_pattern[n_patterns];
    int capacity_combos_by_pattern[n_patterns];
    for (int i = 0; i < n_patterns; i++) {
      combos_by_pattern[i] = NULL;
      n_combos_by_pattern[i] = 0;
      capacity_combos_by_pattern[i] = 0;
    }

    for (int i = 0; i < (1 << n_buttons); i++) {
      int current_joltage[n_reqs];
      memset(current_joltage, 0, sizeof(current_joltage));
      int presses = 0;
      for (int b = 0; b < n_buttons; b++) {
        if ((i >> b) & 1) {
          presses++;
          for (int r = 0; r < n_reqs; r++) {
            if (buttons[b][r])
              current_joltage[r]++;
          }
        }
      }

      int pattern = 0;
      for (int r = 0; r < n_reqs; r++) {
        if (current_joltage[r] % 2 != 0)
          pattern |= (1 << r);
      }

      // Add combo to its pattern bucket
      if (n_combos_by_pattern[pattern] >= capacity_combos_by_pattern[pattern]) {
        capacity_combos_by_pattern[pattern] =
            capacity_combos_by_pattern[pattern] == 0
                ? 4
                : capacity_combos_by_pattern[pattern] * 2;
        combos_by_pattern[pattern] =
            realloc(combos_by_pattern[pattern],
                    sizeof(Combo) * capacity_combos_by_pattern[pattern]);
      }
      combos_by_pattern[pattern][n_combos_by_pattern[pattern]].joltage =
          malloc(sizeof(int) * n_reqs);
      memcpy(combos_by_pattern[pattern][n_combos_by_pattern[pattern]].joltage,
             current_joltage, sizeof(int) * n_reqs);
      combos_by_pattern[pattern][n_combos_by_pattern[pattern]].presses =
          presses;
      n_combos_by_pattern[pattern]++;
    }

    // Recursive search with memoization
    clear_cache();
    count +=
        count_presses(joltage_goal, combos_by_pattern, n_combos_by_pattern);

    // Clean up memory for this machine
    for (int i = 0; i < n_patterns; i++) {
      for (int j = 0; j < n_combos_by_pattern[i]; j++) {
        free(combos_by_pattern[i][j].joltage);
      }
      if (combos_by_pattern[i])
        free(combos_by_pattern[i]);
    }
  }

  printf("Total min presses: %lld\n", count);
  fclose(f);
  return 0;
}
