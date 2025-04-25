#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Defining the PI constant
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void loadFile(const char* filename, float* filter, int size);
void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate);
void writeCSV(const char* filename, const float* data, int size);
void applyFIR(const float* signal, float* output, const float* filter, float* capturedOutput, int size);

int main() {
    const unsigned int size = 2000;

    // Allocate memory for filter, signal, and output
    float *filter = malloc(size * sizeof(float));
    float *signal = malloc(size * sizeof(float));
    float *output = malloc(size * sizeof(float));
    float *capturedOutput = malloc(size * sizeof(float));

    // Error handling for memory allocation
    if (!filter || !signal || !output || !capturedOutput) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Load filter coefficients
    loadFile("filter.bin", filter, size);

    // Frequency sweep parameters
    const float waveAmplitude = 1.0;
    const float sampleRate = 1.0;
    const float startFrequency = 0.2e-3;
    const float frequencyStep = 1e-5;
    const float endFrequency = 0.01;

    const int numFrequencies = (int)((endFrequency - startFrequency) / frequencyStep) + 1;

    // Allocate memory for output in dB and frequency array
    float *outputDB = malloc(numFrequencies * sizeof(float));
    float *frequencies = malloc(numFrequencies * sizeof(float));

    if (!outputDB || !frequencies) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int freqIndex = 0;
    for (float freq = startFrequency; freq <= endFrequency; freq += frequencyStep) {
        //Generate sine wave signal
        generateSineWave(signal, size, freq, waveAmplitude, sampleRate);

        //Apply FIR filter
        applyFIR(signal, output, filter, capturedOutput, size);

        //Find maximum amplitude of the output
        float maxAmplitude = 0.0;
        for (int i = 0; i < size; i++) {
            if (fabs(output[i]) > maxAmplitude) {
                maxAmplitude = fabs(output[i]);
            }
        }

        //Store amplitude in dB and frequency
        outputDB[freqIndex] = 20 * log10(maxAmplitude);
        frequencies[freqIndex] = freq;

        freqIndex++;
    }

    // Write results to CSV files
    writeCSV("output/fir_amplitude.csv", outputDB, numFrequencies);
    writeCSV("output/fir_frequency.csv", frequencies, numFrequencies);

    // Free allocated memory
    free(signal);
    free(output);
    free(outputDB);
    free(frequencies);
    free(capturedOutput);

    // Reallocate memory for larger signal processing
    const unsigned int largeSize = 49000;
    signal = malloc(largeSize * sizeof(float));
    output = malloc(largeSize * sizeof(float));
    capturedOutput = malloc(largeSize * sizeof(float));

    if (!signal || !output || !capturedOutput) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    // Load filter and signal
    loadFile("signal.bin", signal, largeSize);

    // Apply FIR filter
    applyFIR(signal, output, filter, capturedOutput, largeSize);

    // Write results to CSV files
    writeCSV("output/capturedOutput.csv", capturedOutput, largeSize);
    writeCSV("output/signal.csv", signal, largeSize);

    // Free allocated memory
    free(filter);
    free(output);
    free(signal);
    free(capturedOutput);

    return 0;
}

void loadFile(const char* filename, float* filter, int size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (fread(filter, sizeof(float), size, file) != size) {
        perror("Error reading file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}

void writeCSV(const char* filename, const float* data, int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%f\n", data[i]);
    }

    fclose(file);
}

void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate) {
    const float angularFrequency = 2 * M_PI * waveFrequency / sampleRate;
    for (int i = 0; i < size; i++) {
        signal[i] = waveAmplitude * sin(angularFrequency * i);
    }
}

void applyFIR(const float* signal, float* output, const float* filter, float* capturedOutput, int size) {
    for (int i = 0; i < size; i++) {
        float acc = 0.0;
        for (int j = 0; j < 2000; j++) {
            if (i >= j) {
                acc += filter[j] * signal[i - j];
            }
        }
        output[i] = acc;
        capturedOutput[i] = acc;
    }
}
