// Common typedefs

#pragma once

typedef unsigned char byte, uint8;
typedef char int8;
typedef unsigned short uint16, wchar;
typedef short int16;
typedef long int32;
typedef unsigned long uint32;
typedef long long int64;
typedef unsigned long long uint64;

#ifndef __cplusplus
typedef unsigned char bool;
enum { false=0, true };
#endif
