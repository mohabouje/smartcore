from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class MedianFilter(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("MedianFilter",
                         "Perform a median filter on an input signal")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("kernel_size", "Size of the median filter window in each dimension", int(0), False)

    def run(self, x: ndarray):
        return signal.medfilt(x, kernel_size=self._value("kernel_size"))
