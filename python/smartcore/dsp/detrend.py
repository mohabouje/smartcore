from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm


class DeTrend(Algorithm):

    def __init__(self, algorithm_type: str = "linear", name: str = "", timestamp: int = 0):
        self.algorithm_type: str = algorithm_type
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return signal.detrend(x, type=self.algorithm_type)
