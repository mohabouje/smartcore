import sys
import PySide2
import numpy as np
import threading
import pyqtgraph as pg

from argparse import ArgumentParser
from time import sleep
from pyaudio import PyAudio, paFloat32, paContinue
from smartcore.dsp import cepstrum, periodogram, preemphasis, windower, detrend
from smartcore.core.audio_buffer import AudioBuffer
from smartcore.chart import linear_chart
from pyqtgraph.Qt import QtGui



class Processor:

    def __init__(self, buffer_size: int, fft_size: int, fs: int, winlen: float, winstep: float):
        self.audio_buffer = AudioBuffer(buffer_size=buffer_size, dtype=np.float, winlen=winlen,  winstep=winstep, fs=fs)
        self.audio_buffer.initialize()

        self.dc = detrend.DeTrend(algorithm_type="constant")
        self.dc.initialize()

        self.pe = preemphasis.PreEmphasis(coefficient=0.95)
        self.pe.initialize()

        self.win = windower.Window(window="hamming", window_size=self.audio_buffer.window_length)
        self.win.initialize()

        self.ceps = cepstrum.Cepstrum()
        self.ceps.initialize()

        self.spec = periodogram.Periodogram(fs=8000, fft_size=fft_size)
        self.spec.initialize()

        self.fig = linear_chart.LinearChart(title="Example")
        self.fig.initialize()

    def audio_processing(self, mic_data, frame_count, time_info, flag):
        in_data = np.fromstring(mic_data, np.float)
        frames = self.audio_buffer.run(in_data)
        for frame in frames:
            if np.count_nonzero(frame) is not 0:
                frame = self.dc.run(frame)
                frame = self.pe.run(frame)
                frame = self.win.run(frame)
                freq, spectrum = self.spec.run(frame)
                self.fig.run(freq, spectrum)
        return mic_data, paContinue


def start_recording(fs: int, buffer_size: int, processor: Processor):
    pa = PyAudio()
    stream = pa.open(format=paFloat32, channels=1, frames_per_buffer=buffer_size,
                     rate=fs, input=True, stream_callback=processor.audio_processing)

    while stream.is_active():
        sleep(0.1)

    stream.close()
    pa.terminate()


def main():
    parser = ArgumentParser(description="Process the microphone input in real time.")
    parser.add_argument("fs", type=float, nargs='?', default=8000, help="Sampling rate in Hz.")
    parser.add_argument("channels", type=int, nargs='?',  default=1, help="Number of channels.")
    parser.add_argument("buffer", type=float, nargs='?', default=0.04, help="Time of each buffer in seconds.")
    parser.add_argument("index", type=int, nargs='?', default=0, help="Number of frames per buffer.")
    parser.add_argument("winlen", type=float, nargs='?', default=0.025, help="Time of each frame in seconds.")
    parser.add_argument("winstep", type=float, nargs='?', default=0.005, help="Time of each step in seconds.")
    args = parser.parse_args()

    buffer_size = int(args.buffer * args.fs)
    processor = Processor(buffer_size=buffer_size, fs=args.fs, fft_size=1024,
                          winlen=args.winlen, winstep=args.winstep)
    audio_thread = threading.Thread(target=start_recording, args=(args.fs, buffer_size, processor))
    audio_thread.start()
    audio_thread.join()

    pg.QtGui.QGuiApplication.instance().exec_()


if __name__ == "__main__":
    main()
