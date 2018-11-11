from smartcore.core.algorithm import Algorithm
from speechpy.processing import preemphasis


class PreEmphasis(Algorithm):

    def __init__(self, coefficient: float, name: str = "", timestamp: int = 0):
        self.coefficient: float = coefficient
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return preemphasis(signal=x, cof=self.coefficient)