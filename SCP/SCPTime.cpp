#include "SCPTime.h"
#include "BytesTool.h"

using namespace Communication.IO.Tools;

namespace ECGConversion.SCP
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
SCPTime::SCPTime(int hour, int min, int sec)
{
    Hour = (byte) hour;
    Min = (byte) min;
    Sec = (byte) sec;
}

/// <summary>
/// Function to write SCP time.
/// </summary>
/// <param name="buffer">byte array to write into</param>
/// <param name="offset">position to start writing</param>
/// <returns>0 on success</returns>
int SCPTime::Write(byte[] buffer, int offset)
{
    if ((offset + Size) > buffer.Length)
    {
        return 0x1;
    }

    BytesTool::writeBytes(Hour, buffer, offset, sizeof(Hour), true);
    offset += sizeof(Hour);
    BytesTool::writeBytes(Min, buffer, offset, sizeof(Min), true);
    offset += sizeof(Min);
    BytesTool::writeBytes(Sec, buffer, offset, sizeof(Sec), true);
    offset += sizeof(Sec);

    return 0x0;
}

}
