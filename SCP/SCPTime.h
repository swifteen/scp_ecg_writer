#ifndef _SCPTIME_H_
#define _SCPTIME_H_
#include "ScpGlobal.h"

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// class containing time in SCP format.
/// </summary>
class SCPTime
{
public:
	SCPTime();
    /// <summary>
    /// Constructor of a SCP time.
    /// </summary>
    /// <param name="hour">number of hour</param>
    /// <param name="min">number of minute</param>
    /// <param name="sec">number of second</param>
    SCPTime(int hour, int min, int sec);

    /// <summary>
    /// Function to write SCP time.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer,int bufferLength, int offset);
public:
    static int Size;
    uchar Hour ;
    uchar Min ;
    uchar Sec ;
};
}
}
#endif
