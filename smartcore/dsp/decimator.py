from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class Decimator(Algorithm):

    def __init__(self, q: int, n: int, filter_type: str = "iir", zero_phase: bool = True, name: str = "", timestamp: int = 0):
        self.q: int = q
        self.n: int = n
        self.filter_type: str = filter_type
        self.zero_phase: bool = zero_phase
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return signal.decimate(x, q=self.q, n=self.n, ftype=self.filter_type, zero_phase=self.zero_phase)
