# pylint: disable=missing-module-docstring,missing-class-docstring,missing-function-docstring  # noqa: E501
import unittest
import py_z85 as uut


class PyZ85Tests(unittest.TestCase):

    def test_py_z85(self) -> None:
        test_data_1 = bytes.fromhex('864FD26FB559F75B')
        test_data_2 = bytes.fromhex(
            '8E0BDD697628B91D'
            '8F245587EE95C5B0'
            '4D48963F79259877'
            'B49CD9063AEAD3B7')

        encoded = uut.z85enc(test_data_1)
        self.assertEqual(encoded, b'HelloWorld')
        decoded = uut.z85dec(encoded)
        self.assertEqual(decoded, test_data_1)

        encoded = uut.z85enc(test_data_2)
        self.assertEqual(encoded, b'JTKVSB%%)wK0E.X)V>+}o?pNmC{O&4W4b!Ni{Lh6')
        decoded = uut.z85dec(encoded)
        self.assertEqual(decoded, test_data_2)

        with self.assertRaisesRegex(ValueError, r'must be a multiple of 4'):
            uut.z85enc(b'abc')

        with self.assertRaisesRegex(ValueError, r'must be a multiple of 5'):
            uut.z85dec(b'abcd')

        with self.assertRaises(TypeError):
            uut.z85enc("foo")  # type: ignore[arg-type]

        with self.assertRaises(TypeError):
            uut.z85dec("foo")  # type: ignore[arg-type]
