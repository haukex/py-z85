#ifndef __SPEC_32_H__
#define __SPEC_32_H__

#include <stdlib.h>

char *Z85_encode (unsigned char *data, size_t size);

unsigned char *Z85_decode (char *string);

#endif  // #ifndef __SPEC_32_H__