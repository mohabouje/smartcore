from scipy import signal
from numpy import ndarray, append
from python.smartcore.core.algorithm import Algorithm


class PreEmphasis(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("PreEmphasis",
                         "Perform preemphasis on the input signal")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("coeff", "The preemphasis coefficient. Default is 0.95.", float(0.95), False)

    def run(self, x: ndarray):
        return append(x[0], x[1:] - self._value("coeff") * signal[:-1])
