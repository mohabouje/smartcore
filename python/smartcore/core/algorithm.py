from abc import ABC, abstractmethod
from typing import Set
from .parameter_descriptor import ParameterDescriptor
from .parameter_holder import ParameterHolder


class Algorithm(ABC):

    @abstractmethod
    def __iter__(self, name: str, description: str, version: str):
        self.__name = name
        self.__description = description
        self.__version = version
        self.__parameters = ParameterHolder()
        self.__descriptors: Set[ParameterDescriptor] = []
        self.__initialized = False
        self.__configure()
        super.__init__()

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
    def descriptors(self) -> Set[ParameterDescriptor]:
        return self.__descriptors

    @property
    def parameters(self) -> ParameterHolder:
        return self.__parameters

    @abstractmethod
    @parameters.setter
    def parameters(self, parameters: ParameterHolder):
        self.__parameters = parameters

    @abstractmethod
    def initialize(self) -> None:
        self.__validate_parameters()

    @abstractmethod
    def run(self, *arguments):
        pass

    @abstractmethod
    def __configure(self):
        pass

    def __add_descriptor(self, name: str, description: str, default, optional: bool = False):
        if name in self.descriptors:
            raise KeyError("Descriptor %s already defined" % name)
        self.descriptors.add(ParameterDescriptor(name, description, default, optional))

    def __delete_descriptor(self, name: str):
        if name not in self.descriptors:
            raise KeyError("Descriptor %s not found" % name)
        self.descriptors.remove(name)

    def __validate_parameters(self):
        wrong = set()
        missed = set()
        for descriptor in self.descriptors:
            if descriptor.optional:
                if descriptor.name not in self.parameters:
                    self.parameters.insert(descriptor.name, descriptor.default)
                elif descriptor.meta is not self.parameters[descriptor.name].meta:
                    self.parameters.delete(descriptor.name)
                    wrong.add(descriptor)
            elif descriptor.name in self.parameters and descriptor.meta is not self.parameters[descriptor.name].meta:
                wrong.add(descriptor)
            else:
                missed.add(descriptor)
        if not wrong and not missed:
            self.__initialized = True
        else:
            raise AttributeError("Missing parameters: " + repr(missed) + "\n Wrong parameters: " + repr(wrong))

