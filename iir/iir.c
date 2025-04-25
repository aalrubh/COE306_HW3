#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void loadFile(const char* filename, float* signal, int size);
void writeCSV(const char* filename, float* data, int size);
void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate);
void applyIIR(float* signal, float* output, float* capturedOutput, float b0, float b1, float a1, int size);

float bandwidthFrequency = 0.0026;

int main() {
    //Filter parameters
    float radialFrequency = 2 * M_PI * bandwidthFrequency;
    float b0 = radialFrequency / (2 + radialFrequency);
    float b1 = radialFrequency / (2 + radialFrequency);
    float a1 = (radialFrequency - 1) / (radialFrequency + 2);

    //Signal and filter settings
    unsigned int size = 2000;
    float waveAmplitude = 1.0;
    float sampleRate = 1.0;

    const float startFrequency = 0.2e-3;
    const float frequencyStep = 1e-5;
    const float endFrequency = 0.01;

    int numFrequencies = (int)((endFrequency - startFrequency) / frequencyStep) + 1;

    //Allocate memory
    float* signal = (float*)malloc(size * sizeof(float));
    float* output = (float*)malloc(size * sizeof(float));
    float* capturedOutput = (float*)malloc(size * sizeof(float));
    float* outputDB = (float*)malloc(numFrequencies * sizeof(float));
    float* frequencies = (float*)malloc(numFrequencies * sizeof(float));

    // Error handling for memory allocation
    if (!signal || !output || !capturedOutput || !outputDB || !frequencies) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    //Applying IIR filter to a range of frequencies
    int freqIndex = 0;
    for (float freq = startFrequency; freq <= endFrequency; freq += frequencyStep) {
        generateSineWave(signal, size, freq, waveAmplitude, sampleRate);
        applyIIR(signal, output, capturedOutput, b0, b1, a1, size);

        float maxAmplitude = 0.0;
        for (int i = 0; i < size; i++) {
            if (fabs(output[i]) > maxAmplitude) {
                maxAmplitude = fabs(output[i]);
            }
        }

        outputDB[freqIndex] = 20 * log10(maxAmplitude);
        frequencies[freqIndex] = freq;
        freqIndex++;
    }

    //Write results to CSV files
    writeCSV("output/iir_amplitude.csv", outputDB, numFrequencies);
    writeCSV("output/iir_frequency.csv", frequencies, numFrequencies);

    //Free memory
    free(signal);
    free(output);
    free(outputDB);
    free(frequencies);
    free(capturedOutput);

    //Process signal from file
    size = 49000;
    signal = (float*)malloc(size * sizeof(float));
    output = (float*)malloc(size * sizeof(float));
    capturedOutput = (float*)malloc(size * sizeof(float));

    if (!signal || !output || !capturedOutput) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    loadFile("signal.bin", signal, size);
    applyIIR(signal, output, capturedOutput, b0, b1, a1, size);
    writeCSV("output/capturedOutput_iir.csv", capturedOutput, size);

    free(signal);
    free(output);
    free(capturedOutput);

    return 0;
}

void writeCSV(const char* filename, float* data, int size) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Failed to open output file");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < size; i++) {
        fprintf(file, "%f\n", data[i]);
    }

    fclose(file);
}

void generateSineWave(float* signal, int size, float waveFrequency, float waveAmplitude, float sampleRate) {
    for (int i = 0; i < size; i++) {
        signal[i] = waveAmplitude * sin(2 * M_PI * waveFrequency * i / sampleRate);
    }
}

void applyIIR(float* signal, float* output, float* capturedOutput, float b0, float b1, float a1, int size) {
    output[0] = 0.0;
    output[1] = 0.0;

    for (int i = 2; i < size; i++) {
        output[i] = b0 * signal[i] + b1 * signal[i - 1] - a1 * output[i - 1];
        capturedOutput[i] = output[i];
    }
}

void loadFile(const char* filename, float* signal, int size) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    if (fread(signal, sizeof(float), size, file) != size) {
        perror("Error reading file");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
}
