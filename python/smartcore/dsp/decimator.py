from scipy import signal
from python.smartcore.core.algorithm import Algorithm


class Decimator(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Convolver",
                         "Computes the convolution of two input signals.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("q", "The downsampling factor", int(0), False)
        self._add_descriptor("n", "The order of the anti-aliasing  filter", int(8), True)
        self._add_descriptor("type", "Specifies the type of lowpass filter [irr, fir]", "iir", True)
        self._add_descriptor("zero_phase", "If true, prevents phase shift", True, True)

    def run(self, x):
        return signal.decimate(x, q=self._value("q"), n=self._value("n"),
                               ftype=self._value("type"), zero_phase=self._value("zero_phase"))
