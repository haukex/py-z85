Python Wrapper for Reference Implementation of Z85
==================================================

This code tries to remain as true to the reference implementation
of <https://rfc.zeromq.org/spec/32/> as possible.

`pip install 'py-z85@git+https://github.com/haukex/py-z85.git'`

    >>> from py_z85 import z85enc, z85dec
    >>> z85enc(bytes.fromhex('864FD26FB559F75B'))
    b'HelloWorld'
    >>> z85dec(b'HelloWorld').hex()
    '864fd26fb559f75b'

Note
----

Python provides
[`base64.z85encode`](https://docs.python.org/3/library/base64.html#base64.z85encode)
and
[`base64.z85decode`](https://docs.python.org/3/library/base64.html#base64.z85decode).
This package exists so implementors of Z85 en-/decoders can more easily
access the reference implementation to check their implementations against.


Legal
-----

The file `py_z85.c` is derived from
<https://github.com/zeromq/rfc/blob/8416923d/src/spec_32.c>
and contains the following legal information.

The code has since been modified by Hauke Daempfling (haukex@zero-g.net), Dec 2025.

Copyright (c) 2010-2013 iMatix Corporation and Contributors

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
