from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class DeConvolver(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("De-Convolver",
                         "De-convolves an impulse response signal out of a signal using inverse filtering.")
        self.parameters = parameters

    def _configure(self):
        pass

    def run(self, x: ndarray, impulse_response: ndarray):
        return signal.convolve(x, impulse_response)