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
    #plt.ylim(-6, 6)

def main():
    #   Loading the signal data
    amplitude = pd.read_csv(amplitude_file, header=None)[0]
    time = range(len(amplitude))

    #   Plotting the signal
    configure_plot()
    plt.plot(time, amplitude, label='Signal', color='blue')
    plt.show()
    
main()