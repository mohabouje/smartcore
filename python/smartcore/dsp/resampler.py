from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class ReSampler(Algorithm):

    def __init__(self, up: int, down: int, window: str = "hamming", name: str = "", timestamp: int = 0):
        self.up: int = up
        self.down: int = down
        self.window: str = window
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return signal.resample_poly(x, up=self.up, down=self.down, window=self.window)
