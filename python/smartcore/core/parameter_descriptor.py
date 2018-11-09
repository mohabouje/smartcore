class ParameterDescriptor:

    def __init__(self, name: str, description: str, default: object, optional: bool = False) -> None:
        self.__name = name
        self.__description = description
        self.__meta = type(default)
        self.__default = default
        self.__optional = optional
        super().__init__()

    def __str__(self):
        return "{ name : %s, description: %s, meta : %s, optional : %r } " % (self.name,
                                                                              self.description,
                                                                              self.meta.__name__,
                                                                              self.optional)

    @property
    def optional(self) -> bool:
        return self.__optional

    @property
    def name(self) -> str:
        return self.__name

    @property
    def description(self) -> str:
        return self.__description

    @property
    def meta(self) -> type:
        return self.__meta

    @property
    def default(self) -> object:
        return self.default

    @name.setter
    def name(self, value: str):
        self.__name = value

    @description.setter
    def description(self, value: str):
        self.__description = value

    @optional.setter
    def optional(self, value: bool):
        self.__optional = value

    @meta.setter
    def meta(self, value: type):
        self.__meta = value

    @default.setter
    def default(self, value: Any):
        if not isinstance(value, self.__meta):
            raise TypeError("Expecting type " + self.__meta.__name__)
        self.__default = value
