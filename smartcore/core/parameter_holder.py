from typing import Dict
from smartcore.core.parameter import Parameter
from smartcore.util import serializer


class ParameterHolder:

    def __init__(self):
        self.__parameters: Dict[str, Parameter] = {}
        super().__init__()

    def __len__(self) -> int:
        return len(self.parameters)

    def __setitem__(self, key: str, value: Parameter):
        if self.contains(key):
            raise KeyError("Key %s previously used" % key)
        self.parameters[key] = value

    def __getitem__(self, key: str) -> Parameter:
        return self.parameters[key]

    def __delitem__(self, key: str) -> None:
        self.parameters.pop(key, None)

    def __contains__(self, key: str) -> bool:
        return key in self.parameters

    def __str__(self) -> str:
        return serializer.serialize(self.parameters)

    def insert(self, name: str, default: object):
        self.__setitem__(name, Parameter(name, default))

    def delete(self, key: str):
        self.__delitem__(key)

    def contains(self, key: str):
        return self.__contains__(key)

    @property
    def parameters(self) -> dict:
        return self.__parameters

    @parameters.setter
    def parameters(self, parameters):
        if parameters is None:
            return

        for key, value in parameters.iteritems():
            self.parameters[key] = Parameter(key, value)
