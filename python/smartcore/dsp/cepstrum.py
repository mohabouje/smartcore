from acoustics import cepstrum
from smartcore.core.algorithm import Algorithm


class Cepstrum(Algorithm):

    def __init__(self, fft_size: int = None, name: str = "", timestamp: int = 0):
        self.fft_size: int = fft_size
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return cepstrum.real_cepstrum(x, n=self.fft_size)
