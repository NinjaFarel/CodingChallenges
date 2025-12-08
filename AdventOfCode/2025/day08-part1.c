#include <stdio.h>
#include <stdlib.h>

#define N_BOXES 1000
#define N_CONN 1000
#define TOP 3

typedef struct Box {
  int x;
  int y;
  int z;
} Box;

typedef struct Distance {
  long long amount;
  int a;
  int b;
} Distance;

typedef struct Circuit {
  int parent;
  int rank;
  int size;
} Circuit;

int cmp_dist(const void *d1p, const void *d2p) {
  Distance *d1 = (Distance *)d1p;
  Distance *d2 = (Distance *)d2p;

  if (d1->amount < d2->amount)
    return -1;
  if (d1->amount > d2->amount)
    return 1;
  return 0;
}

int cmp_size(const void *c1p, const void *c2p) {
  Circuit *c1 = (Circuit *)c1p;
  Circuit *c2 = (Circuit *)c2p;

  if (c1->size < c2->size)
    return 1;
  if (c1->size > c2->size)
    return -1;
  return 0;
}

int find_parent(Circuit circuits[], int i) {
  if (circuits[i].parent != i)
    circuits[i].parent = find_parent(circuits, circuits[i].parent);
  return circuits[i].parent;
}

int main() {
  FILE *f = fopen("input08.txt", "r");

  Box boxes[N_BOXES];
  Distance distances[(N_BOXES * (N_BOXES - 1)) / 2];
  Circuit circuits[N_BOXES];
  int count = 1;

  // Kruskal's algorithm (or something similar)
  // Read input and create circuits
  for (int i = 0; i < N_BOXES; i++) {
    fscanf(f, "%d,%d,%d", &boxes[i].x, &boxes[i].y, &boxes[i].z);
    circuits[i] = (Circuit){i, 0, 1};
  }

  // Calculate and sort all the possible distances
  int idx = 0;
  for (int i = 0; i < N_BOXES - 1; i++) {
    for (int j = i + 1; j < N_BOXES; j++) {
      distances[idx].a = i;
      distances[idx].b = j;
      long long dx = boxes[i].x - boxes[j].x;
      long long dy = boxes[i].y - boxes[j].y;
      long long dz = boxes[i].z - boxes[j].z;
      distances[idx++].amount = (dx * dx) + (dy * dy) + (dz * dz);
    }
  }
  qsort(distances, (N_BOXES * (N_BOXES - 1)) / 2, sizeof(distances[0]),
        cmp_dist); // Probably less efficent than a quickselect

  // Join circuits
  for (int i = 0; i < N_CONN; i++) {
    int a_parent = find_parent(circuits, distances[i].a);
    int b_parent = find_parent(circuits, distances[i].b);

    if (a_parent == b_parent)
      continue; // Skip if already in the same circuit

    if (circuits[a_parent].rank < circuits[b_parent].rank) {
      circuits[a_parent].parent = b_parent;
      circuits[b_parent].size += circuits[a_parent].size;
    } else if (circuits[a_parent].rank > circuits[b_parent].rank) {
      circuits[b_parent].parent = a_parent;
      circuits[a_parent].size += circuits[b_parent].size;
    } else {
      circuits[b_parent].parent = a_parent;
      circuits[a_parent].rank++;
      circuits[a_parent].size += circuits[b_parent].size;
    }
  }

  // Again, probably not efficent, but the problem is already very hard
  qsort(circuits, N_BOXES, sizeof(circuits[0]), cmp_size);

  for (int i = 0; i < TOP; i++) {
    count *= circuits[i].size;
  }

  printf("Product of top %d's sizes: %lld\n", TOP, count);
  fclose(f);
}