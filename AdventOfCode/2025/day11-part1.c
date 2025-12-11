#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DEVICES 2000
#define MAX_NAME 10
#define MAX_OUTPUTS 10

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
long long count_paths(int idx) {
  if (strcmp(devices[idx].name, "out") == 0) {
    return 1;
  }

  if (devices[idx].cached_paths != -1) {
    return devices[idx].cached_paths;
  }

  long long paths = 0;
  for (int i = 0; i < devices[idx].n_outputs; i++) {
    int next_idx = find_device(devices[idx].outputs[i]);
    if (next_idx != -1) {
      paths += count_paths(next_idx);
    } else if (strcmp(devices[idx].outputs[i], "out") == 0) {
      // If the target device isn't defined as a key but is "out"
      paths += 1;
    }
  }

  devices[idx].cached_paths = paths;
  return paths;
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

    // Parse outputs
    char *outputs_str = find_end + 1;
    FILE *outputs_file = fmemopen(outputs_str, strlen(outputs_str), "r");
    char output_name[MAX_NAME];
    while (fscanf(outputs_file, "%s", output_name) != EOF) {
      strcpy(devices[idx].outputs[devices[idx].n_outputs++], output_name);
    }
    fclose(outputs_file);
  }

  // Find start device "you"
  int start_idx = find_device("you");
  if (start_idx == -1) {
    printf("Device 'you' not found.\n");
    fclose(f);
    return 1;
  }

  // Calculate total paths
  long long total_paths = count_paths(start_idx);

  printf("Total paths from you to out: %lld\n", total_paths);

  fclose(f);
  return 0;
}
