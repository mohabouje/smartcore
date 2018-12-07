from smartcore.core.algorithm import Algorithm
from numpy_ringbuffer import RingBuffer
from numpy import array


class CircularBuffer(Algorithm):

    def __init__(self, capacity: int, dtype, value=None, name: str = "", timestamp: int = 0):
        self.__deque = RingBuffer(capacity=capacity, dtype=dtype)
        self.__fill(value)
        super().__init__(name, timestamp)

    def __fill(self, value):
        for i in range(0, self.__deque.maxlen):
            self.__deque.append(value)

    def run(self, x):
        for i in x:
            self.__deque.append(i)
        return array(self.__deque)

    @property
    def capacity(self) -> int:
        return self.__deque.maxlen

    @property
    def data(self) -> RingBuffer:
        return self.__deque
