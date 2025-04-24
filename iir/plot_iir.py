#   Loading External Libraries
import pandas as pd
import matplotlib.pyplot as plt
import os

#   Set the working directory to the script's directory
os.chdir(os.path.dirname(os.path.abspath(__file__)))

#   The file names that would be used
amplitude_file = 'output/iir_amplitude.csv'
frequency_file = 'output/iir_frequency.csv'

#   The threshold value
threshold = -3

def find_bandwidth_frequency(amplitudes, frequencies, threshold):
    for i in range(len(amplitudes)):
        if amplitudes[i] <= threshold:
            return frequencies[i]
    return None

def configure_plot():
    plt.xscale('log')
    plt.xlabel('Frequency (Hz)')
    plt.ylabel('Amplitude (dB)')
    plt.title('Amplitude versus Frequency')
    plt.grid()


def main():
    #   Loading data from CSV files using pandas
    amplitudes = pd.read_csv(amplitude_file, header=None)[0]
    frequencies = pd.read_csv(frequency_file, header=None)[0]

    #   Finding the bandwidth frequency
    bandwidth_frequency = find_bandwidth_frequency(amplitudes, frequencies, threshold)

    #   Printing the bandwidth frequency
    print("Bandwidth Frequency: %.4f Hz" % bandwidth_frequency)

    #   Configuring the plot
    configure_plot()
    plt.plot(frequencies, amplitudes, label='Amplitude vs Frequency')
    plt.axvline(bandwidth_frequency, color='red', linestyle='dotted', label= "-3 dB at %.4f Hz" % bandwidth_frequency)
    plt.legend()
    plt.show()

main()
