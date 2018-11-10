from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class ReSampler(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("DeSampler",
                         "Resamples a signal using polyphase filtering.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("up", "The up-sampling factor", int(0), False)
        self._add_descriptor("down", "The down-sampling factor", int(0), False)
        self._add_descriptor("window", "Specifies the window applied to the signal", "hamming", True)

    def run(self, x: ndarray):
        return signal.resample_poly(x, up=self._value("up"), down=self._value("down"), window=self._value("window"))
