from numpy import ndarray
from acoustics import cepstrum
from python.smartcore.core.algorithm import Algorithm


class Windower(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Cepstrum",
                         "Computes the cepstrum of the input signal.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("nfft", "Length of the FFT used", int(0), True)

    def run(self, x: ndarray):
        nfft = self._value("nfft")
        return cepstrum.real_cepstrum(x, n=nfft if nfft > 0 else len(x))
