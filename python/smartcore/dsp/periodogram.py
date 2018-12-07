from scipy import signal
from smartcore.core.algorithm import Algorithm


class Periodogram(Algorithm):

    def __init__(self, fs: float, fft_size: int = None, window: str = "hamming", scaling: str = "density",
                 name: str = "", timestamp: int = 0):
        self.fs: float = fs
        self.fft_size: int = fft_size
        self.window: str = window
        self.scaling: str = scaling
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return signal.periodogram(x, fs=self.fs, nfft=self.fft_size, window=self.window, scaling=self.scaling)
