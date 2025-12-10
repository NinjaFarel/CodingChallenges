#include <stdbool.h>
#include <stdio.h>
#include <string.h>

int main() {
  FILE *f = fopen("input10.txt", "r");
  int count = 0;
  char buff[1000];

  while (fgets(buff, sizeof(buff), f) != NULL) {
    // Read input
    char *find_start, *find_end;
    find_end = strchr(buff, ']');
    int n_lights = (int)(find_end - buff) - 1;

    bool lights_goal[n_lights];
    for (int i = 0; i < n_lights; i++) {
      lights_goal[i] = buff[i + 1] == '#';
    }

    int n_buttons = 0;
    for (int i = 0; buff[i] != '\0'; i++) {
      if (buff[i] == '(')
        n_buttons++;
    }

    bool buttons[n_lights][n_buttons + 1];
    memset(buttons, 0, sizeof(buttons));
    find_start = buff;
    for (int i = 0; i < n_buttons; i++) {
      find_start = strchr(find_start + 1, '(');
      find_end = strchr(find_start, ')');
      FILE *button = fmemopen(find_start + 1, find_end - find_start - 1, "r");

      int n = 0;
      while (fscanf(button, "%d", &n) != EOF) {
        buttons[n][i] = true;
        getc(button);
      }
      fclose(button);
    }

    // Augment the matrix
    for (int r = 0; r < n_lights; r++) {
      buttons[r][n_buttons] = lights_goal[r];
    }

    // Apply Gaussian elimination (mod 2)
    int p_row = 0, p_col = 0;

    while (p_row < n_lights && p_col < n_buttons) {
      int i_pivot = -1;

      // Find pivot
      for (int i = p_row; i < n_lights; i++) {
        if (buttons[i][p_col] == 1) {
          i_pivot = i;
          break;
        }
      }

      if (i_pivot == -1) {
        // No pivot
        p_col++;
      } else {
        // Swap pivot row to top
        if (p_row != i_pivot) {
          for (int j = p_col; j < n_buttons + 1; j++) {
            bool temp = buttons[p_row][j];
            buttons[p_row][j] = buttons[i_pivot][j];
            buttons[i_pivot][j] = temp;
          }
        }

        // Elimination (with XOR)
        for (int i = p_row + 1; i < n_lights; i++) {
          if (buttons[i][p_col] == 1) {
            for (int j = p_col; j < n_buttons + 1; j++) {
              buttons[i][j] = buttons[i][j] ^ buttons[p_row][j];
            }
          }
        }
        p_row++;
        p_col++;
      }
    }

    // Find solutions (p_col is the rank of the matrix)
    int pivot_cols[n_lights];
    bool is_free_col[n_buttons];
    for (int j = 0; j < n_buttons; j++)
      is_free_col[j] = true;

    for (int i = 0; i < p_row; i++) {
      for (int j = 0; j < n_buttons; j++) {
        if (buttons[i][j]) {
          pivot_cols[i] = j;
          is_free_col[j] = false;
          break;
        }
      }
    }

    int free_variables[n_buttons];
    int n_free = 0;
    for (int j = 0; j < n_buttons; j++) {
      if (is_free_col[j])
        free_variables[n_free++] = j;
    }

    // Brute force the values to find the solution with less presses
    int min_presses = n_buttons + 1;
    int free_values[n_free > 0 ? n_free : 1];
    memset(free_values, 0, sizeof(free_values));

    while (true) {
      int current_sol[n_buttons];
      int current_presses = 0;

      for (int k = 0; k < n_free; k++) {
        int col = free_variables[k];
        current_sol[col] = free_values[k];
        if (free_values[k])
          current_presses++;
      }

      for (int i = p_row - 1; i >= 0; i--) {
        int pc = pivot_cols[i];
        int val = buttons[i][n_buttons];

        for (int j = pc + 1; j < n_buttons; j++) {
          if (buttons[i][j] && current_sol[j]) {
            val ^= 1;
          }
        }

        current_sol[pc] = val;
        if (val)
          current_presses++;
      }

      if (current_presses < min_presses) {
        min_presses = current_presses;
      }

      if (n_free == 0)
        break;

      int k = 0;
      while (k < n_free) {
        if (free_values[k] == 0) {
          free_values[k] = 1;
          break;
        } else {
          free_values[k] = 0;
          k++;
        }
      }
      if (k == n_free)
        break;
    }

    count += min_presses;
  }

  printf("Button presses: %d\n", count);
  fclose(f);
  return 0;
}