from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class Convolver(Algorithm):

    def __init__(self, mode: str = "full", method: str = "auto", name: str = "", timestamp: int = 0):
        self.mode = mode
        self.method = method
        super().__init__(name=name, timestamp=timestamp)

    def run(self, input1, input2):
        return signal.convolve(input1, input2, mode=self.mode, method=self.method)
