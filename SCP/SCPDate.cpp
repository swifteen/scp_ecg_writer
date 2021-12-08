#include "SCPDate.h"

namespace ECGConversion.SCP
{
/// <summary>
/// class containing date in SCP format.
/// </summary>


/// <summary>
/// Constructor of a SCP date.
/// </summary>
SCPDate::SCPDate():Size(4)
{
	Year = 0;
	Month = 0;
	Day = 0;
}
/// <summary>
/// Constructor of a SCP date.
/// </summary>
/// <param name="year">number of year</param>
/// <param name="month">number of month</param>
/// <param name="day">number of day</param>
SCPDate::SCPDate(int year, int month, int day)
{
    Year = (ushort) year;
    Month = (byte) month;
    Day = (byte) day;
}

/// <summary>
/// Function to write an SCP date to a byte array.
/// </summary>
/// <param name="buffer">byte array to write in</param>
/// <param name="offset">position to start writing</param>
/// <returns></returns>
int SCPDate::Write(byte[] buffer, int offset)
{
    if ((offset + Size) > buffer.Length)
    {
        return 0x1;
    }

    BytesTool.writeBytes(Year, buffer, offset, Marshal.SizeOf(Year), true);
    offset += Marshal.SizeOf(Year);
    BytesTool.writeBytes(Month, buffer, offset, Marshal.SizeOf(Month), true);
    offset += Marshal.SizeOf(Month);
    BytesTool.writeBytes(Day, buffer, offset, Marshal.SizeOf(Day), true);
    offset += Marshal.SizeOf(Day);

    return 0x0;
}

}
