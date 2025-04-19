
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

void loadFile(float* filter);
void generateSineWave(float* signal);
void writeCSV(float* data);

/*
    The parameters for the filter
*/
char* inputFile = "filter.bin";
char* outputFile = "output.csv";

float waveFrequency = 0.1;
float waveAmplitude = 1.0;
float sampleRate = 1.0;

int size = 2000;

int main() {
    float* filter = (float*)malloc(size * sizeof(float));

    if (!filter) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    float* signal = (float*)malloc(size * sizeof(float));

    if (!signal) {
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

/*
    This function will be used to load the contents of filter binary file into an array.
*/
void loadFile(float* filter) {
    FILE *file = fopen(inputFile, "rb");

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
    This function will be used to write the output values into a CSV file to use for plotting.
*/
void writeCSV(float* data) {

    FILE *file = fopen(outputFile, "w");

    if (!file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%f\n", data[i]);
    }

    fclose(file);
}

/*
    This function will be used to generate a sine wave signal at a specific sampling rate.
*/
void generateSineWave(float* signal) {
    for (int i = 0; i < size; i++) {
        signal[i] = waveAmplitude * sin(2 * M_PI * waveFrequency * i / sampleRate);
    }
}



