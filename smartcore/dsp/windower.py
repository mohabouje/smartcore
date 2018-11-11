from scipy import signal
from numpy import ndarray, zeros
from smartcore.core.algorithm import Algorithm


class Window(Algorithm):

    def __init__(self, window: str, window_size: int, name: str = "", timestamp: int = 0):
        self.window: str = window
        self.window_size: int = window_size
        self.__data = zeros(shape=[window_size])
        super().__init__(name=name, timestamp=timestamp)

    def initialize(self):
        self.__data = signal.get_window(window=self.window, Nx=self.window_size)
        super().initialize()

    def run(self, x):
        if len(x) != self.window_size:
            raise ValueError("Expected input data with %d samples but the input has %d samples"
                             % (self.window_size, x.size))
        return x * self.__data

    @property
    def data(self) -> ndarray:
        return self.__data
