from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class Correlator(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Correlator",
                         "Computes the correlation of two input signals.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("mode", "String indicating the size of the output [full, valid, same]", "full", True)
        self._add_descriptor("method", "String indicating which method to use [auto, direct, fft]", "auto", True)

    def run(self, input1: ndarray, input2: ndarray):
        return signal.correlate(input1, input2, mode=self._value("mode"), method=self._value("method"))



