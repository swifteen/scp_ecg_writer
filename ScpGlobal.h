#ifndef _SCPGLOBAL_H_
#define _SCPGLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>

#ifndef HAVE_NO_TYPEDEF_UCHAR
#define HAVE_NO_TYPEDEF_UCHAR
typedef unsigned char uchar;
typedef unsigned short ushort;


#endif

#ifndef HAVE_NO_TYPEDEF_NULL
#define HAVE_NO_TYPEDEF_NULL
#define null             0L
#endif

#endif  /*#ifndef _SCPGLOBAL_H_*/
