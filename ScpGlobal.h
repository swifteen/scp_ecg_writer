#ifndef _SCPGLOBAL_H_
#define _SCPGLOBAL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>
#include <vector>
#include <time.h> 

using namespace std;

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define uchar_MIN (0)
#define uchar_MAX (uchar(-1))

#define ushort_MinValue (0)
#define ushort_MaxValue (ushort(-1))

#define uint_MIN (0)
#define uint_MAX (uint(-1))

#define ulong_MIN (0)
#define ulong_MAX (ulong(-1L))

#define ulong_MIN (0)
#define ulong_MAX (ulong(-1L))

#define Int32_MaxValue ((int)0x7FFFFFFF)
#define Int32_MinValue ((int)0x80000000)


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


static void scp_print_local_time()
{
    time_t now;
    struct tm  *timenow;	
    char local_time[64];
    memset(local_time, 0, sizeof(local_time));
    time(&now);						
    timenow = localtime(&now);	
    snprintf(local_time, sizeof(local_time),"%04d-%02d-%02d %02d:%02d:%02d", 
        timenow->tm_year + 1900, timenow->tm_mon + 1, timenow->tm_mday,
        timenow->tm_hour, timenow->tm_min, timenow->tm_sec);
    printf("[%s]", local_time);
    return;
}

#define SCP_PRINT_LOCAL_TIME scp_print_local_time();

#define SCP_DUMP_RED printf("\033[0;32;31m")
#define SCP_DUMP_YELLOW printf("\033[1;33m")
#define SCP_DUMP_GREEN printf("\033[0;32;32m")
#define SCP_DUMP_NONE printf("\033[m")

#define SCP_PD(msg, args...)        do{SCP_DUMP_GREEN; SCP_PRINT_LOCAL_TIME;printf("[SCP0] DEBUG! %s::%s::%d: ", __FILE__, __FUNCTION__, __LINE__);printf(msg, ##args);SCP_DUMP_NONE;}while(0)
#define SCP_PW(msg, args...)        do{SCP_DUMP_YELLOW; SCP_PRINT_LOCAL_TIME;printf("[SCP0] WARNING! %s::%s::%d: ", __FILE__, __FUNCTION__, __LINE__);printf(msg, ##args);SCP_DUMP_NONE;}while(0)
#define SCP_PE(msg, args...)        do{SCP_DUMP_RED; SCP_PRINT_LOCAL_TIME;printf("[SCP0] ERROR! %s::%s::%d: ", __FILE__, __FUNCTION__, __LINE__);printf(msg, ##args);SCP_DUMP_NONE;}while(0)

#endif  /*#ifndef _SCPGLOBAL_H_*/
