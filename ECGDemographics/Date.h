#ifndef _ZQ_DATE_H_
#define _ZQ_DATE_H_

namespace ECGConversion.ECGDemographics
{
/// <summary>
/// Class containing a date (format is equal to SCP).
/// </summary>
class Date
{
public：
    /// <summary>
    /// Constructor to make a date.
    /// </summary>
    Date();
    /// <summary>
    /// Constructor to make a date.
    /// </summary>
    /// <param name="year">number of year</param>
    /// <param name="month">number of month</param>
    /// <param name="day">number of day</param>
    Date(ushort year, byte month, byte day);

    /// <summary>
    /// Check if date is likely to be an existing date.
    /// </summary>
    /// <returns>true: is an existing date.
    /// false: is an non existing date.</returns>
    bool isExistingDate();
public：
    // Content of date class.
    ushort Year ;
    byte Month ;
    byte Day ;
private:
    // Static information for check.
    static byte[] _DaysInMonth ;
    static int _LeapMonth ;
    static byte _DaysInLeapMonth ;
};
}
