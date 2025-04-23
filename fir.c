#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//Defining the PI constant, had issues where M_PI was not defined and produced errors
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <unistd.h>

void loadFile(float* filter);
void generateSineWave(float* signal);
void writeCSV(float* data);
void applyFIR(float* signal, float* filter, float* output);

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
    //Allocating memory for filter, signal, and output
    float* filter = (float*)malloc(size * sizeof(float));
    float* signal = (float*)malloc(size * sizeof(float));
    float* output = (float*)malloc(size * sizeof(float));

    //Error handling for memory allocation
    if (!filter || !signal || !output) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    //Loading the coefficients from the filter file
    loadFile(filter);

    //Generating a sine wave signal
    generateSineWave(signal);

    //Applying the FIR filter to the signal
    applyFIR(signal, filter, output);

    //Writing the output to a CSV file
    writeCSV(output);

    //printing for debugging purposes
    for (int i = 0; i < 10; i++) {
        printf("y[%d] = %f\n", i, output[i]);
    }

    //Freeing the allocated memory
    free(filter);
    free(signal);
    free(output);

    return 0;
}

/*
    This function will be used to load the contents of filter binary file into an array.
*/
void loadFile(float* filter) {
    //Creating a file pointer
    FILE *file = fopen(inputFile, "rb");

    //Error handling for file opening
    if (!file) {
        perror("Failed to open filter file");
        exit(EXIT_FAILURE);
    }

    //Reading the size of the filter from the file
    if (fread(filter, sizeof(float), size, file) != size) {
        printf("Error reading the file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    //Closing the file
    fclose(file);
}

/*
    This function will be used to write the output values into a CSV file to use for plotting.
*/
void writeCSV(float* data) {
    //Creating a file pointer
    FILE *file = fopen(outputFile, "w");

    //Error handling for file opening
    if (!file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    //Writing contents to the file
    for (int i = 0; i < size; i++) {
        fprintf(file, "%f\n", data[i]);
    }

    //Closing the file
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

/*
    This function will be used to apply the FIR filter to the signal.
*/
void applyFIR(float* signal, float* filter, float* output) {
    //For each sample in the signal
    for (int n = 0; n < size; n++) {
        //Resetting the output
        output[n] = 0.0;

        //For each coefficient in the filter
        for (int k = 0; k < size; k++) {
            //Applying the filter in the subsequent samples
            if (n - k >= 0) {
                output[n] += filter[k] * signal[n - k];
            }
        }
    }
}
