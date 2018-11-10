from numpy import ndarray
from acoustics import cepstrum
from smartcore.core.algorithm import Algorithm


class Cepstrum(Algorithm):

    def __init__(self, nfft=None):
        super().__init__("Cepstrum",
                         "Computes the cepstrum of the input signal.")
        self.nfft: int = nfft

    def _configure(self):
        self._add_descriptor("nfft", "Length of the FFT used", type(int))

    def run(self, x: ndarray):
        nfft = self._value("nfft")
        return cepstrum.real_cepstrum(x, n=nfft if nfft > 0 else len(x))
