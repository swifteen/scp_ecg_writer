#ifndef _SCPGLOBAL_H_
#define _SCPGLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <vector>

using namespace std;

typedef unsigned char uchar;

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
