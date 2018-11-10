from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class Windower(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Periodogram",
                         "Computes the periodogram of the input signals.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("fs", "Sampling frequency in Hz.", float(), False)
        self._add_descriptor("nfft", "Length of the FFT used", int(0), True)
        self._add_descriptor("window", "Desired window to use.", "hamming", True)
        self._add_descriptor("scaling", "Scaling factor: [density, spectrum]", "density", True)

    def run(self, x: ndarray):
        nfft = self._value("nfft")
        return signal.periodogram(x, fs=self._value("fs"), nfft=nfft if nfft > 0 else len(x),
                                  window=self._value("window"), scaling=self._value("scaling"))
