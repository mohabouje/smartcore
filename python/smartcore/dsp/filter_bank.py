from numpy import ndarray
from python_speech_features import fbank, logfbank
from python.smartcore.core.algorithm import Algorithm


class FilterBank(Algorithm):

    def __init__(self, parameters: dict = None):
        super().__init__("FilterBank",
                         "Compute Mel-filterbank energy features from an audio signal.")
        self.parameters = parameters

    def _configure(self):
        self._add_descriptor("fs", "Sampling rate in Hz", int(), False)
        self._add_descriptor("winlen", "Length of the analysis windows in seconds", float(0.025), True)
        self._add_descriptor("winstep", "Step between successive windows in seconds.", float(0.01), True)
        self._add_descriptor("nfilt", "Number of filters in the filter bank", int(26), True)
        self._add_descriptor("nfft", "FFT size", int(512), True)

    def run(self, x: ndarray):
        return fbank(x, samplerate=self._value("fs"), winlen=self._value("winlen"), winstep=self._value("winstep"),
                     nfilt=self._value("nfilt"), nfft=self._value("nfft"), preemph=0)