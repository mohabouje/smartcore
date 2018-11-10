class Parameter:

    def __init__(self, name: str, value: object) -> None:
        self.__name = name
        self.__meta = type(value)
        self.__value = value
        super().__init__()

    def __str__(self) -> str:
        return "{%s : % s} " % (self.name, self.value)

    @property
    def name(self) -> str:
        return self.__name

    @property
    def value(self) -> object:
        return self.__value

    @property
    def meta(self) -> type:
        return self.__meta

    @name.setter
    def name(self, value: str):
        self.__name = value

    @value.setter
    def value(self, value: object):
        if not isinstance(value, self.__meta):
            try:
                self.__value = self.__meta(value)
            except ValueError:
                raise ValueError("Expecting type " + self.__meta.__name__)
        self.__value = value

