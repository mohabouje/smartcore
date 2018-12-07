from smartcore.core.circular_buffer import CircularBuffer
from numpy import ndarray, arange, searchsorted
from speechpy.processing import stack_frames


class AudioBuffer(CircularBuffer):

    def __init__(self, buffer_size: int, fs: int, winlen: float,
                 winstep: float, dtype, name: str = "", timestamp: int = 0):

        self.__fs = fs
        self.__window_time = winlen
        self.__step_time = winstep
        self.__stride_time = self.__window_time - self.__step_time

        window_length = round(winlen * fs)
        step_length = round(winstep * fs)
        candidates = arange(0, buffer_size, step_length)
        last_possible_index = buffer_size - window_length
        remaining_index = searchsorted(candidates, last_possible_index, side='right')
        remaining = buffer_size - candidates[remaining_index]
        capacity = buffer_size + remaining
        self.__processing_length = candidates[remaining_index] + window_length
        super().__init__(capacity=capacity, dtype=dtype, name=name, timestamp=timestamp, value=0.0)



    @property
    def window_length(self) -> int:
        return round(self.__window_time * self.__fs)

    @property
    def step_length(self) -> int:
        return round(self.__step_time * self.__fs)

    def run(self, x):
        return stack_frames(sig=super().run(x)[:self.__processing_length], sampling_frequency=self.__fs,
                            frame_length=self.__window_time, frame_stride=self.__step_time, zero_padding=True)

