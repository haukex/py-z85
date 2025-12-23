// This file is based on https://github.com/zeromq/rfc/blob/8416923d/src/spec_32.c

//  --------------------------------------------------------------------------
//  Reference implementation for rfc.zeromq.org/spec:32/Z85
//
//  This implementation provides a Z85 codec as an easy-to-reuse C class
//  designed to be easy to port into other languages.
//  --------------------------------------------------------------------------
//  Copyright (c) 2010-2013 iMatix Corporation and Contributors
//
//  (With modifications by Hauke Daempfling <haukex@zero-g.net>, Dec 2025)
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//  --------------------------------------------------------------------------

// https://docs.python.org/3.10/extending/index.html
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

//  Maps base 256 to base 85
static char encoder [85 + 1] = {
    "0123456789"
    "abcdefghij"
    "klmnopqrst"
    "uvwxyzABCD"
    "EFGHIJKLMN"
    "OPQRSTUVWX"
    "YZ.-:+=^!/"
    "*?&<>()[]{"
    "}@%$#"
};

//  Maps base 85 to base 256
//  We chop off lower 32 and higher 128 ranges
static unsigned char decoder [96] = {
    0x00, 0x44, 0x00, 0x54, 0x53, 0x52, 0x48, 0x00,
    0x4B, 0x4C, 0x46, 0x41, 0x00, 0x3F, 0x3E, 0x45,
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x40, 0x00, 0x49, 0x42, 0x4A, 0x47,
    0x51, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A,
    0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32,
    0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A,
    0x3B, 0x3C, 0x3D, 0x4D, 0x00, 0x4E, 0x43, 0x00,
    0x00, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10,
    0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
    0x21, 0x22, 0x23, 0x4F, 0x00, 0x50, 0x00, 0x00
};

//  --------------------------------------------------------------------------

static PyObject *Z85_encode(PyObject *Py_UNUSED(self), PyObject *args) {
    const unsigned char *data;
    Py_ssize_t size;
    // https://docs.python.org/3.10/c-api/arg.html#strings-and-buffers
    if (!PyArg_ParseTuple(args, "y#", &data, &size))
        return NULL;

    if (size % 4) {
        PyErr_SetString(PyExc_ValueError, "data length must be a multiple of 4");
        return NULL;
    }

    Py_ssize_t encoded_size = size * 5 / 4;
    char *encoded = (char*) PyMem_Malloc(encoded_size);
    if (encoded==NULL) {
        PyErr_SetString(PyExc_MemoryError, "malloc failed");
        return NULL;
    }

    Py_ssize_t char_nbr = 0;
    Py_ssize_t byte_nbr = 0;
    uint32_t value = 0;
    while (byte_nbr < size) {
        //  Accumulate value in base 256 (binary)
        value = value * 256 + data [byte_nbr++];
        if (byte_nbr % 4 == 0) {
            //  Output value in base 85
            unsigned int divisor = 85 * 85 * 85 * 85;
            while (divisor) {
                encoded [char_nbr++] = encoder [value / divisor % 85];
                divisor /= 85;
            }
            value = 0;
        }
    }
    assert (char_nbr == encoded_size);
    // https://docs.python.org/3.10/c-api/bytes.html#c.PyBytes_FromStringAndSize
    PyObject *rv = PyBytes_FromStringAndSize(encoded, encoded_size);
    PyMem_Free(encoded);
    return rv;
}

//  --------------------------------------------------------------------------

static PyObject *Z85_decode(PyObject *Py_UNUSED(self), PyObject *args) {
    const char *string;
    Py_ssize_t size;
    // https://docs.python.org/3.10/c-api/arg.html#strings-and-buffers
    if (!PyArg_ParseTuple(args, "y#", &string, &size))
        return NULL;

    if (size % 5) {
        PyErr_SetString(PyExc_ValueError, "data length must be a multiple of 5");
        return NULL;
    }

    Py_ssize_t decoded_size = size * 4 / 5;
    char *decoded = (char*) PyMem_Malloc(decoded_size);
    if (decoded==NULL) {
        PyErr_SetString(PyExc_MemoryError, "malloc failed");
        return NULL;
    }

    Py_ssize_t byte_nbr = 0;
    Py_ssize_t char_nbr = 0;
    uint32_t value = 0;
    while (char_nbr < size) {
        //  Accumulate value in base 85
        value = value * 85 + decoder [(unsigned char) string [char_nbr++] - 32];
        if (char_nbr % 5 == 0) {
            //  Output value in base 256
            unsigned int divisor = 256 * 256 * 256;
            while (divisor) {
                decoded [byte_nbr++] = value / divisor % 256;
                divisor /= 256;
            }
            value = 0;
        }
    }
    assert (byte_nbr == decoded_size);
    // https://docs.python.org/3.10/c-api/bytes.html#c.PyBytes_FromStringAndSize
    PyObject *rv = PyBytes_FromStringAndSize(decoded, decoded_size);
    PyMem_Free(decoded);
    return rv;
}

static PyMethodDef py_z85_methods[] = {
    {"z85enc", Z85_encode, METH_VARARGS, "Encode a bytes-like object using Z85. The object's length must be multiple of 4."},
    {"z85dec", Z85_decode, METH_VARARGS, "Decode a bytes-like object using Z85. The object's length must be multiple of 5."},
    {NULL, NULL, 0, NULL}  // Sentinel
};

static struct PyModuleDef_Slot module_slots[] = {
#if PY_VERSION_HEX >= 0x030D0000
    {Py_mod_gil, Py_MOD_GIL_USED},
#endif
    {0, NULL}  // Sentinel
};

static struct PyModuleDef py_z85_module = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "py-z85",
    .m_size = 0,
    .m_methods = py_z85_methods,
    .m_slots = module_slots
};

PyMODINIT_FUNC
PyInit_py_z85(void) {
    return PyModuleDef_Init(&py_z85_module);
}

/* vim: set ts=4 sw=4 expandtab : */