

class Parameter:

    def __init__(self, name: str, description: str, value, optional : bool = False) -> None:
        self.__name = name
        self.__description = description
        self.__meta = type(value)
        self.__value = value
        self.__optional = optional
        super().__init__()

    def __eq__(self, other):
        if isinstance(other, self.__class__):
            return self.__dict__ == other.__dict__
        elif isinstance(other, str):
            return self.name == other
        else:
            return False

    def __str__(self):
        return "{%s : % s} " % (self.name, self.value)

    @property
    def optional(self):
        return self.__optional

    @property
    def name(self) -> str:
        return self.__name

    @property
    def description(self) -> str:
        return self.__description

    @property
    def value(self):
        return self.__value

    @property
    def meta(self):
        return self.__meta

    @name.setter
    def name(self, value : str):
        self.__name = value

    @description.setter
    def description(self, value : str):
        self.__description = value

    @value.setter
    def value(self, value):
        if not isinstance(value, self.__meta):
            raise ValueError("Expecting type " + self.__meta.__name__)
        self.__value = value

    @optional.setter
    def optional(self, value : bool):
        self.__optional = value
