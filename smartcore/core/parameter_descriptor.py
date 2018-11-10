class ParameterDescriptor:

    def __init__(self, name: str, description: str, meta: type, default: object = None, optional: bool = False) -> None:
        if meta is not type(default):
            raise ValueError("Expecting default value of type %s but %s is provided"
                             % (meta.__name__, type(default).__name__))
        self.__name = name
        self.__description = description
        self.__meta = meta
        self.__default = default
        self.__optional = optional
        super().__init__()

    def __str__(self):
        return '{ "name" : "%s", "description": "%s", "meta" : "%s", "default":  "%s", "optional" : "%r" } ' \
               % (self.name, self.description, self.meta.__name__, self.default, self.optional)

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
        return self.__default

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
    def default(self, value: object):
        if not isinstance(value, self.__meta):
            raise TypeError("Expecting type " + self.__meta.__name__)
        self.__default = value
