from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class DeTrend(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("DeTrend",
                         "Remove linear trend along axis from data.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("type", "The type of de-trending [linear, constant]", "linear", True)
        self._add_descriptor("bp", "A sequence of break points", ndarray(), True)

    def run(self, x: ndarray):
        return signal.detrend(x, type=self._value("type"), bp=self._value("bp"))
