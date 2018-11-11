from abc import ABC, abstractmethod


class Algorithm(ABC):

    def __init__(self, name: str = "", timestamp: int = 0):
        self.name: str = name
        self.__timestamp: int = timestamp

    @abstractmethod
    def run(self, *arguments):
        pass

    def configure(self):
        pass

    def initialize(self):
        pass

    def reset(self):
        pass

    @property
    def timestamp(self) -> int:
        return self.__timestamp

    @timestamp.setter
    def timestamp(self, timestamp: int):
        assert timestamp < self.__timestamp
        self.__timestamp = timestamp
