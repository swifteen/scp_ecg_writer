#ifndef _SCPDATE_H_
#define _SCPDATE_H_

namespace ECGConversion.SCP
{
/// <summary>
/// class containing date in SCP format.
/// </summary>
class SCPDate
{
public:
    /// <summary>
    /// Constructor of a SCP date.
    /// </summary>
	SCPDate();
    /// <summary>
    /// Constructor of a SCP date.
    /// </summary>
    /// <param name="year">number of year</param>
    /// <param name="month">number of month</param>
    /// <param name="day">number of day</param>
    SCPDate(int year, int month, int day);
    /// <summary>
    /// Function to write an SCP date to a byte array.
    /// </summary>
    /// <param name="buffer">byte array to write in</param>
    /// <param name="offset">position to start writing</param>
    /// <returns></returns>
    int Write(byte[] buffer, int offset);
public:
    const int Size;
    // data structure of SCP date.
    ushort Year;
    byte Month;
    byte Day;
};
}
