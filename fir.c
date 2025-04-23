#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//Defining the PI constant, had issues where M_PI was not defined and produced errors
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void loadFile(const char* filename, float* filter, int size);
void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate);
void writeCSV(const char* filename, float* data, int size);
void applyFIR(float* signal, float* filter, float* output, int size);

int main() {
    //Allocating memory for filter, signal, and output
    unsigned int size = 2000;

    float* filter = (float*)malloc(size * sizeof(float));
    float* signal = (float*)malloc(size * sizeof(float));
    float* output = (float*)malloc(size * sizeof(float));

    //Error handling for memory allocation
    if (!filter || !signal || !output) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    //Loading the coefficients from the filter file
    loadFile("filter.bin", filter, size);

    // Loop over frequencies and find the maximum amplitude of the output
    float waveAmplitude = 1.0;
    float sampleRate = 1.0;

    float startFrequency = 1e-3;
    float frequencyStep = 1e-3;
    float endFrequency = 0.5;

    int numFrequencies = (int)((endFrequency - startFrequency) / frequencyStep) + 1;

    //Allocating memory for the output in dB and frequency array
    float* outputDB = (float*)malloc(numFrequencies * sizeof(float));
    float* frequencies = (float*)malloc(numFrequencies * sizeof(float));

    int freqIndex = 0;
    for (float freq = startFrequency; freq <= endFrequency; freq += frequencyStep) {
        //Generate a sine wave signal for the current frequency
        generateSineWave(signal, size, freq, waveAmplitude, sampleRate);

        //Apply the FIR filter to the signal
        applyFIR(signal, filter, output, size);

        //Find the maximum amplitude of the output
        float maxAmplitude = 0.0;
        for (int i = 0; i < size; i++) {
            if (fabs(output[i]) > maxAmplitude) {
                maxAmplitude = fabs(output[i]);
            }
        }

        //Calculate amplitude in dB and store it in the output array
        outputDB[freqIndex] = 20 * log10(maxAmplitude);

        //Store the frequency value in the frequencies array
        frequencies[freqIndex] = freq;

        freqIndex++;
    }

    //Writing the output to CSV files
    writeCSV("fir_amplitude.csv", outputDB, numFrequencies);
    writeCSV("fir_frequency.csv", frequencies, numFrequencies);

    //Freeing the allocated memory
    free(filter);
    free(signal);
    free(output);
    free(outputDB);
    free(frequencies);

    return 0;
}

/*
    This function will be used to load the contents of filter binary file into an array.
*/
void loadFile(const char* filename, float* filter, int size) {
    //Creating a file pointer
    FILE* file = fopen(filename, "rb");

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
void writeCSV(const char* filename, float* data, int size) {
    //Creating a file pointer
    FILE* file = fopen(filename, "w");

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
void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate) {
    for (int i = 0; i < size; i++) {
        signal[i] = waveAmplitude * sin(2 * M_PI * waveFrequency * i / sampleRate);
    }
}

/*
    This function will be used to apply the FIR filter to the signal.
*/
void applyFIR(float* signal, float* filter, float* output, int size) {
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
