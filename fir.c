//
// Created by Ali on 2025-04-18.
//

#include <stdio.h>
#include <stdlib.h>

void loadFile(const char *filename, float* filter, int size) {
  FILE *file = fopen(filename, "rb");

  if (!file) {
    perror("Failed to open filter file");
    exit(EXIT_FAILURE);
  }

  if (fread(filter, sizeof(float), size, file) != size) {
    printf("Error reading the file");
    fclose(file);
    exit(EXIT_FAILURE);
  }
  fclose(file);
}

/*
implement an FIR filter (considering the sampling
rate is 1 Hz, for simplicity). You may use the circular buffer implementaion to perform the
convolution of the filter coefficients with the input signal
*/



int main() {
  char* filename = "filter.bin";
  int size = 2000;

  float* filter =(float*)malloc(size * sizeof(float));

  if (!filter) {
    perror("Memory allocation failed");
    exit(EXIT_FAILURE);
  }

  loadFile(filename, filter, size);

  for (int i = 0; i < size; i++) {
    printf("%f\n", filter[i]);
  }

  free(filter);
  return 0;
}