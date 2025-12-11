#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 3000
#define MAX_NAME 10
#define MAX_OUTPUTS 30 // Increased to handle devices with many outputs
#define INF 1000000000000000LL

typedef struct Device {
  char name[MAX_NAME];
  char outputs[MAX_OUTPUTS][MAX_NAME];
  int n_outputs;
  long long cached_paths;
} Device;

Device devices[MAX_DEVICES];
int device_count = 0;

// Find device index by name
int find_device(const char *name) {
  for (int i = 0; i < device_count; i++) {
    if (strcmp(devices[i].name, name) == 0) {
      return i;
    }
  }
  return -1;
}

// Get device index, create if it doesn't exist
int get_or_create_device(const char *name) {
  int idx = find_device(name);
  if (idx != -1)
    return idx;

  strcpy(devices[device_count].name, name);
  devices[device_count].n_outputs = 0;
  devices[device_count].cached_paths = -1;
  return device_count++;
}

// Recursive function to count paths with memoization
long long count_paths(int current_idx, int target_idx) {
  if (current_idx == target_idx) {
    return 1;
  }

  if (devices[current_idx].cached_paths != -1) {
    return devices[current_idx].cached_paths;
  }

  long long paths = 0;
  for (int i = 0; i < devices[current_idx].n_outputs; i++) {
    int next_idx = find_device(devices[current_idx].outputs[i]);
    if (next_idx != -1) {
      paths += count_paths(next_idx, target_idx);
    }
  }

  devices[current_idx].cached_paths = paths;
  return paths;
}

// Helper to reset cache for a new target calculation
void reset_cache() {
  for (int i = 0; i < device_count; i++) {
    devices[i].cached_paths = -1;
  }
}

int main() {
  FILE *f = fopen("input11.txt", "r");
  if (!f)
    return 1;

  char buff[1000];
  // Read input
  while (fgets(buff, sizeof(buff), f) != NULL) {
    char name[MAX_NAME];
    char *find_end = strchr(buff, ':');
    if (!find_end)
      continue;

    // Extract device name
    int name_len = find_end - buff;
    strncpy(name, buff, name_len);
    name[name_len] = '\0';

    int idx = get_or_create_device(name);

    // Parse outputs using your preferred style
    char *outputs_str = find_end + 1;
    FILE *outputs_file = fmemopen(outputs_str, strlen(outputs_str), "r");
    char output_name[MAX_NAME];
    while (fscanf(outputs_file, "%s", output_name) != EOF) {
      if (devices[idx].n_outputs < MAX_OUTPUTS) {
        strcpy(devices[idx].outputs[devices[idx].n_outputs++], output_name);
        // Ensure the output device is also registered
        get_or_create_device(output_name);
      }
    }
    fclose(outputs_file);
  }

  // Find required devices for the problematic data path
  int svr_idx = find_device("svr");
  int dac_idx = find_device("dac");
  int fft_idx = find_device("fft");
  int out_idx = find_device("out");

  if (svr_idx == -1 || dac_idx == -1 || fft_idx == -1 || out_idx == -1) {
    printf("One or more required devices (svr, dac, fft, out) not found.\n");
    fclose(f);
    return 1;
  }

  // Calculate paths for both possible orderings in the DAG

  // Case 1: svr -> dac -> fft -> out
  reset_cache();
  long long svr_to_dac = count_paths(svr_idx, dac_idx);
  reset_cache();
  long long dac_to_fft = count_paths(dac_idx, fft_idx);
  reset_cache();
  long long fft_to_out = count_paths(fft_idx, out_idx);
  long long case1 = svr_to_dac * dac_to_fft * fft_to_out;

  // Case 2: svr -> fft -> dac -> out
  reset_cache();
  long long svr_to_fft = count_paths(svr_idx, fft_idx);
  reset_cache();
  long long fft_to_dac = count_paths(fft_idx, dac_idx);
  reset_cache();
  long long dac_to_out = count_paths(dac_idx, out_idx);
  long long case2 = svr_to_fft * fft_to_dac * dac_to_out;

  // Final result is the sum of these mutually exclusive path counts
  printf("Total paths from svr to out visiting both dac and fft: %lld\n",
         case1 + case2);

  fclose(f);
  return 0;
}
