from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class MedianFilter(Algorithm):

    def __init__(self, kernel_size: int, name: str = "", timestamp: int = 0):
        self.kernel_size: int = kernel_size
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return signal.medfilt(x, kernel_size=self.kernel_size)
