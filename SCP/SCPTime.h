#ifndef _SCPTIME_H_
#define _SCPTIME_H_

namespace ECGConversion.SCP
{
/// <summary>
/// class containing time in SCP format.
/// </summary>
class SCPTime
{
public:
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
    int Write(byte[] buffer, int offset);
public:
    const int Size;
    byte Hour ;
    byte Min ;
    byte Sec ;
};
}
