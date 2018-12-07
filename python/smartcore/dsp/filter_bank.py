from numpy import ndarray
from python_speech_features import fbank
from smartcore.core.algorithm import Algorithm


class FilterBank(Algorithm):

    def __init__(self, fs : float, winlen: float = 0.025, winstep: float = 0.01, nfilt: int = 26, fft_size: int = 512, name: str = "", timestamp: int = 0):
        self.fs: float = fs
        self.winlen: float = winlen
        self.winstep: float = winstep
        self.nfilt: int = nfilt
        self.fft_size: int = fft_size
        super().__init__(name=name, timestamp=timestamp)

    def run(self, x):
        return fbank(x, samplerate=self.fs, winlen=self.winlen, winstep=self.winstep,
                     nfilt=self.nfilt, nfft=self.fft_size, preemph=0)
