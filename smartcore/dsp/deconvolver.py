from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class DeConvolver(Algorithm):

    def __init__(self, name: str = "", timestamp: int = 0):
        super().__init__(name=name, timestamp=timestamp)

    def _configure(self):
        pass

    def run(self, x, impulse_response):
        return signal.convolve(x, impulse_response)
