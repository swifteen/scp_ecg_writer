#ifndef _SCPGLOBAL_H_
#define _SCPGLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <vector>

using namespace std;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define uchar_MIN (0)
#define uchar_MAX (uchar(-1))

#define ushort_MIN (0)
#define ushort_MAX (ushort(-1))

#define uint_MIN (0)
#define uint_MAX (uint(-1))

#define ulong_MIN (0)
#define ulong_MAX (ulong(-1L))

#define null             0L

typedef struct
{
	int Year;
	int Month;
	int Day;
	int Hour;
	int Minute;
	int Second;
}DateTime;

#endif  /*#ifndef _SCPGLOBAL_H_*/
