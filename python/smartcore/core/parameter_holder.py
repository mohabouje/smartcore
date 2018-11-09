from typing import Dict
from .parameter import *


class ParameterHolder:

    def __init__(self):
        self.__parameters: Dict[str, Parameter] = {}
        super().__init__()

    def __len__(self):
        return len(self.parameters)

    def __getitem__(self, key: str):
        return self.parameters[key]

    def __delitem__(self, key: str):
        self.parameters.pop(key, None)

    def __contains__(self, key: str):
        return key in self.parameters

    def __str__(self):
        for v in self.parameters.values():
            print(str(v))

    def insert(self, parameter: Parameter):
        if self.contains(parameter.name):
            raise KeyError("Key %s previously used" % parameter.name)
        self.parameters[parameter.name] = parameter

    def insert(self, name: str, default):
        if self.contains(name):
            raise KeyError("Key %s previously used" % name)
        self.parameters[name] = Parameter(name, default)

    def delete(self, key: str):
        self.__delitem__(key)

    def contains(self, key: str):
        return self.__contains__(key)

    @property
    def parameters(self) -> dict:
        return self.__parameters

    @parameters.setter
    def parameters(self, parameters):
        self.__parameters = parameters
