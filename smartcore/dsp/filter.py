from scipy import signal
from numpy import ndarray
from smartcore.core.algorithm import Algorithm

class Filter(Algorithm):

    def __init__(self, wp, ws, gpass: float, gstop: float, analog: bool = True,
                 filter_type: str = "ellip", name: str = "", timestamp: int = 0):
        self.wp = wp
        self.ws = ws
        self.gpass: float = gpass
        self.gstop: float = gstop
        self.analog: bool = analog
        self.filter_type: str = filter_type
        self.__b = None
        self.__a = None
        self.__zf = None
        super().__init__(name=name, timestamp=timestamp)

    def initialize(self):
        self.__b, self.__a, __ = signal.iirdesign(wp=self.wp, ws=self.ws, gpass=self.gpass, gstop=self.gstop,
                                                  analog=self.analog, ftype=self.filter_type)
        super().initialize()

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
