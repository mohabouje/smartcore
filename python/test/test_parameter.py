import unittest
from ..utils import *
from ..core import *


class TestParameterMethods(unittest.TestCase):

    def test_creation(self):
        name = string_generator()
        description = string_generator()
        value = 0
        param = Parameter(name, description, value)

        self.assertEqual(name, param.name)
        self.assertEqual(description, param.description)
        self.assertEqual(value, param.value)
        self.assertEqual(type(value), param.meta)

    def test_update_name(self):
        name = generator.string_generator()
        description = generator.string_generator()
        value = 0
        param = Parameter(name, description, value)

        param.name = generator.string_generator()
        self.assertNotEqual(param.name, name)

        param.name = name
        self.assertEqual(param.name, name)

    def test_invalid_value(self):
        param = Parameter("example", " ", 4578.0)
        self.assertRaises(ValueError, setattr, param, "value", generator.string_generator())

    def test_valid_value(self):
        param = Parameter("example", " ", 4578.0)
        param.value = 74.

