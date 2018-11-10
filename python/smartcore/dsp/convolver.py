from scipy import signal
from python.smartcore.core.algorithm import Algorithm


class Convolver(Algorithm):

    def __init__(self):
        super().__init__("Convolver",
                         "Computes the convolution of two input signals.")

    def _configure(self):
        self._add_descriptor("mode", "String indicating the size of the output [full, valid, same]", "full", True)
        self._add_descriptor("method", "String indicating which method to use [auto, direct, fft]", "auto", True)

    def run(self, input1, input2):
        return signal.convolve(input1, input2, mode=self._value("mode"), method=self._value("method"))
