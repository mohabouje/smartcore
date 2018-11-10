from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class Window(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Convolver",
                         "Computes the convolution of two input signals.")
        self.__window = ndarray()
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("type", "The type of window to create.", str(), False)
        self._add_descriptor("size", "The number of samples in the window.", int(0), False)

    def initialize(self):
        super().initialize()
        self.__window = signal.get_window(window=self._value("type"), Nx=self._value("size"))

    def run(self, x: ndarray):
        if x.size is not self.__window.size:
            raise ValueError("Expected input with %d samples" % self.__window.size)
        return x * self.__window

    @property
    def data(self):
        return self.__window
