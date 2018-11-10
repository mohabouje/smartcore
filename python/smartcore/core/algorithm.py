from abc import ABC, abstractmethod
from typing import Dict
import datetime
from .parameter_descriptor import ParameterDescriptor
from .parameter_holder import ParameterHolder
from .parameter import Parameter


class Algorithm(ABC):

    @abstractmethod
    def __iter__(self, name: str, description: str, version: str, creation_date: datetime = None):
        self.__name = name
        self.__description = description
        self.__creation_date: datetime = creation_date
        self.__version = version
        self.__param: ParameterHolder = None
        self.__descriptors: Dict[str, ParameterDescriptor] = []
        self.__initialized = False
        self.__timestamp: int = 0
        self.__configure()
        super.__init__()

    @abstractmethod
    def initialize(self) -> None:
        self.__validate_parameters()

    @abstractmethod
    def run(self, *arguments):
        pass

    @abstractmethod
    def __configure(self):
        pass

    @property
    def name(self) -> str:
        return self.__name

    @property
    def description(self) -> str:
        return self.__description

    @property
    def version(self) -> str:
        return self.__version

    @property
    def descriptors(self) -> Dict[str, ParameterDescriptor]:
        return self.__descriptors

    @property
    def parameters(self) -> Dict[str, Parameter]:
        return self.__param.parameters

    @property
    def timestamp(self) -> int:
        return self.__timestamp

    @property
    def creation_date(self) -> datetime:
        return self.__creation_date

    @parameters.setter
    def parameters(self, parameters):
        self.__param.parameters = parameters

    @timestamp.setter
    def timestamp(self, timestamp: int):
        assert timestamp < self.__timestamp
        self.__timestamp = timestamp

    def __add_descriptor(self, name: str, description: str, default: object, optional: bool = False):
        if name in self.descriptors:
            raise KeyError("Descriptor %s already defined" % name)
        self.descriptors[name] = ParameterDescriptor(name, description, default, optional)

    def __delete_descriptor(self, name: str):
        if name not in self.descriptors:
            raise KeyError("Descriptor %s not found" % name)
        self.descriptors.pop(name, None)

    def __validate_parameters(self):
        wrong = set()
        missed = set()
        for key, descriptor in self.descriptors:
            if descriptor.optional:
                if key not in self.__param:
                    self.__param.insert(descriptor.name, descriptor.default)
                elif descriptor.meta is not self.__param[descriptor.name].meta:
                    self.__param.delete(descriptor.name)
                    wrong.add(descriptor)
            elif key in self.__param and descriptor.meta is not self.__param[descriptor.name].meta:
                wrong.add(descriptor)
            else:
                missed.add(descriptor)
        self.__initialized = not wrong and not missed
        if not self.__initialized:
            raise AttributeError("Missing parameters: " + repr(missed) + "\n Wrong parameters: " + repr(wrong))

