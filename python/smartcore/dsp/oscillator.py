from scipy import signal
import numpy as np
from smartcore.core.algorithm import Algorithm


class Oscillator(Algorithm):

    def __init__(self, fs: float, f: float, oscillator_type: str,
                 width: float = 1, duty: float = 0.5, phase: float =0, name: str = "", timestamp: int = 0):
        self.fs: float = fs
        self.f: float = f
        self.oscillator_type: str = oscillator_type
        self.width: float = width
        self.duty: float = duty
        self.phase: float = phase
        self.__previous_timestamp = 0
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x: np.ndarray):
        samples = (self.timestamp - self.__previous_timestamp) * self.fs
        t = np.linspace(self.__previous_timestamp, self.timestamp, samples, endpoint=False)
        self.__previous_timestamp = self.timestamp
        if self.oscillator_type is "sawtooth":
            return signal.sawtooth(2 * np.pi * self.f * t, width=self.width)
        elif self.oscillator_type is "square":
            return signal.square(2 * np.pi * self.f * t, duty=self.duty)
        elif self.oscillator_type is "sinusoidal":
            return np.sin(2 * np.pi * self.f * t)
        else:
            raise ValueError("Type %s not recognized" % tp)

    def reset(self):
        self.__previous_timestamp = 0
        super().reset()


