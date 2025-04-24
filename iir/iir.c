/* 
    As per FIR, the bandwidth frequency is 0.0030 Hz
    Therefore, this is the frequency that will be used to build the IIR filter.
*/



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void writeCSV(const char* filename, float* data, int size);
void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate);
void applyIIR(float* signal, float* output, float b0, float b1, float a1, int size);

float bandwidthFrequency = 0.0030;

int main() {
    //Filter Parameters
    float radialFrequency = 2 * M_PI * bandwidthFrequency;

    float b0 = radialFrequency/(2+radialFrequency);
    float b1 = radialFrequency/(2+radialFrequency);
    float a1 = (radialFrequency-1)/(radialFrequency+2);

    //Allocating memory for filter, signal, and output
    unsigned int size = 2000;
    float waveAmplitude = 1.0;
    float sampleRate = 1.0;

    float startFrequency = 1e-3;
    float frequencyStep = 1e-3;
    float endFrequency = 0.5;

    int numFrequencies = (int)((endFrequency - startFrequency) / frequencyStep) + 1;

    //Allocating memory for the signal, and output
    float* signal = (float*)malloc(size * sizeof(float));
    float* output = (float*)malloc(size * sizeof(float));

    //Allocating memory for the output in dB and frequency array
    float* outputDB = (float*)malloc(numFrequencies * sizeof(float));
    float* frequencies = (float*)malloc(numFrequencies * sizeof(float));

    //Error handling for memory allocation
    if (!signal || !output || !outputDB || !frequencies) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int freqIndex = 0;
    for (float freq = startFrequency; freq <= endFrequency; freq += frequencyStep) {
        //Generate a sine wave signal for the current frequency
        generateSineWave(signal, size, freq, waveAmplitude, sampleRate);

        //Apply the FIR filter to the signal
        applyIIR(signal, output, b0, b1, a1, size);

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
    writeCSV("iir_amplitude.csv", outputDB, numFrequencies);
    writeCSV("iir_frequency.csv", frequencies, numFrequencies);

    //Freeing the allocated memory
    free(signal);
    free(output);
    free(outputDB);
    free(frequencies);

    return 0;
}

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

void applyIIR(float* signal, float* output, float b0, float b1, float a1, int size) {
    // Initialize the first two output samples to zero
    output[0] = 0.0;
    output[1] = 0.0;

    // Apply the IIR filter to the signal
    for (int i = 2; i < size; i++) {
        output[i] = b0 * signal[i] + b1 * signal[i - 1] - a1 * output[i - 1];
    }
}
