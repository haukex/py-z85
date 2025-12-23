#ifndef __SPEC_32_H__
#define __SPEC_32_H__

#include <stdlib.h>
typedef unsigned char byte;

char *Z85_encode (byte *data, size_t size);

byte *Z85_decode (char *string);

#endif  // #ifndef __SPEC_32_H__