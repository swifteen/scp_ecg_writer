#include "SCPTime.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Constructor of a SCP time.
/// </summary>
SCPTime::SCPTime():Size(3)
{	
	Hour = 0;
	Min = 0;
	Sec = 0;
}

/// <summary>
/// Constructor of a SCP time.
/// </summary>
/// <param name="hour">number of hour</param>
/// <param name="min">number of minute</param>
/// <param name="sec">number of second</param>
SCPTime::SCPTime(int hour, int min, int sec):Size(10)
{
    Hour = (uchar) hour;
    Min = (uchar) min;
    Sec = (uchar) sec;
}

/// <summary>
/// Function to write SCP time.
/// </summary>
/// <param name="buffer">byte array to write into</param>
/// <param name="offset">position to start writing</param>
/// <returns>0 on success</returns>
int SCPTime::Write(uchar* buffer,int bufferLength,int offset)
{
    if ((offset + Size) > bufferLength)
    {
        return 0x1;
    }

    BytesTool::writeBytes(Hour, buffer, bufferLength,offset, sizeof(Hour), true);
    offset += sizeof(Hour);
    BytesTool::writeBytes(Min, buffer, bufferLength,offset, sizeof(Min), true);
    offset += sizeof(Min);
    BytesTool::writeBytes(Sec, buffer, bufferLength,offset, sizeof(Sec), true);
    offset += sizeof(Sec);

    return 0x0;
}
}
}
