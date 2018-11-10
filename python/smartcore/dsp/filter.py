from scipy import signal
from numpy import ndarray
from python.smartcore.core.algorithm import Algorithm


class Filter(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("Filter",
                         "Filters the input signal with the designed IIR filter")
        self.__b = ndarray()
        self.__a = ndarray()
        self.__zf = None
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("wp", "Passband edge frequencies", ndarray(), False)
        self._add_descriptor("ws", "Stopband edge frequencies", ndarray(), False)
        self._add_descriptor("gpass", "The maximum loss in the passband (dB)", float(), False)
        self._add_descriptor("gstop", "The minimum attenuation in the stopband (dB).", float(), False)
        self._add_descriptor("analog", "When True, return an analog filter.", True, True)
        self._add_descriptor("type", "Type of filter: [butter, cheby1, cheby2, ellip, bessel]", "ellip", True)

    def initialize(self) -> bool:
        initialized = super().initialize()
        self.__b, self.__a = signal.iirdesign(wp=self._value("wp"),
                                              ws=self._value("ws"),
                                              gpass=self._value("gpass"),
                                              gstop=self._value("gstop"),
                                              analog=self._value("analog"),
                                              ftype=self._value("type"))
        return initialized

    def reset(self):
        self.__zf = None

    def run(self, x):
        output, self.__zf = signal.lfilter(self.__a, self.__b, x, zi=self.__zf)
        return output

    @property
    def a(self):
        return self.__a

    @property
    def b(self):
        return self.__b
