import pandas as pd
import matplotlib.pyplot as plt
import os

os.chdir(os.path.dirname(os.path.abspath(__file__)))
amplitude_file = './output/capturedOutput.csv'

def configure_plot():
    plt.figure(figsize=(12, 6))
    plt.xscale('linear')
    plt.xlabel('Time')
    plt.ylabel('Amplitude')
    plt.title('FIR Filtered Signal Plot')
    plt.grid()
    plt.ylim(-6, 6)

def main():
    # Load the signal data from the CSV file without a header
    amplitude = pd.read_csv(amplitude_file, header=None)[0]

    # Generate time values starting from 0 with a step of 1
    time = range(len(amplitude))

    # Plot the signal
    configure_plot()
    plt.plot(time, amplitude, label='Signal', color='blue')
    plt.show()
    
main()