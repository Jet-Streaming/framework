#ifndef _BYTESWAP_H
#define _BYTESWAP_H

#ifdef _WIN32
#include <stdlib.h>
//#include <intrin.h>
#define __bswap_16 _byteswap_ushort
#define __bswap_32 _byteswap_ulong
#define __bswap_64 _byteswap_uint64
#endif

#endif // _BYTESWAP_H
