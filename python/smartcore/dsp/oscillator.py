from scipy import signal
import numpy as np
from python.smartcore.core.algorithm import Algorithm


class Oscillator(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Convolver",
                         "Computes the convolution of two input signals.")
        self.__previous_timestamp = 0
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("fs", "Sampling frequency in Hz.", float(), False)
        self._add_descriptor("f", "Oscillation frequency in Hz.", float(), False)
        self._add_descriptor("type", "The of generated signal [sawtooth, square, sinusoidal]", str(), False)
        self._add_descriptor("width", "Width of the rising ramp as a proportion of the total cycle.", float(1), True)
        self._add_descriptor("duty", "Duty cycle.", float(0.5), True)
        self._add_descriptor("phase", "Phase offset, in degrees, Default: 0", float(0), True)

    def run(self, x: np.ndarray):
        samples = (self.timestamp - self.__previous_timestamp) * self._value("fs")
        t = np.linspace(self.__previous_timestamp, self.timestamp, samples, endpoint=False)
        self.__previous_timestamp = self.timestamp
        tp = self._value("type")
        if tp is "sawtooth":
            return signal.sawtooth(2 * np.pi * self._value("f") * t, width=self._value("width"))
        elif tp is "square":
            return signal.square(2 * np.pi * self._value("f") * t, duty=self._value("duty"))
        elif tp is "sinusoidal":
            return np.sin(2 * np.pi * self._value("f") * t)
        else:
            raise ValueError("Type %s not recognized" % tp)

    def reset(self):
        self.__previous_timestamp = 0


