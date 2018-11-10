from datetime import date
from abc import ABC, abstractmethod
from typing import Dict
from smartcore.core.parameter_descriptor import ParameterDescriptor
from smartcore.core.parameter_holder import ParameterHolder
from smartcore.core.parameter import Parameter


class Algorithm(ABC):

    def __init__(self, name: str, description: str = "", version: str = "", creation_date: date = date.today()):
        self.__name = name
        self.__description = description
        self.__creation_date: date = creation_date
        self.__version = version
        self.__param: ParameterHolder = ParameterHolder()
        self.__descriptors: Dict[str, ParameterDescriptor] = {}
        self.__initialized = False
        self.__timestamp: int = 0

    def __str__(self) -> str:
        return '{"name" : "%s", "description" : "%s", "creation_date" : "%s", "version" : "%s", "parameters" : %s}' \
               % (self.name, self.description, self.creation_date, self.version, self.__param)

    def _value(self, key: str):
        return self.parameters[key].value

    def _add_descriptor(self, name: str, description: str, meta: type, default: object = None, optional: bool = False):
        if name in self.descriptors:
            raise KeyError("Descriptor %s already defined" % name)
        self.descriptors[name] = ParameterDescriptor(name, description, meta,  default, optional)

    def _delete_descriptor(self, name: str):
        if name not in self.descriptors:
            raise KeyError("Descriptor %s not found" % name)
        self.descriptors.pop(name, None)

    def _validate_parameters(self):
        wrong = set()
        missed = set()
        for key, descriptor in self.descriptors.items():
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

    @abstractmethod
    def run(self, *arguments):
        pass

    @abstractmethod
    def _configure(self):
        pass

    def initialize(self):
        self._configure()
        self._validate_parameters()

    def reset(self):
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
    def creation_date(self) -> date:
        return self.__creation_date

    @parameters.setter
    def parameters(self, parameters: dict):
        self.__param.parameters = parameters

    @timestamp.setter
    def timestamp(self, timestamp: int):
        assert timestamp < self.__timestamp
        self.__timestamp = timestamp
