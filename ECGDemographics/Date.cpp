
#include "Date.h"


namespace ECGConversion
{


namespace ECGDemographics
{

/// <summary>
/// Class containing a date (format is equal to SCP).
/// </summary>


uchar Date::_DaysInMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// Static information for check.
int Date::_LeapMonth = 2;
uchar Date::_DaysInLeapMonth = 29;


/// <summary>
/// Constructor to make a date.
/// </summary>
Date::Date()
{
    // Content of date class.
    Year = 0;
    Month = 0;
    Day = 0;
}

/// <summary>
/// Constructor to make a date.
/// </summary>
/// <param name="year">number of year</param>
/// <param name="month">number of month</param>
/// <param name="day">number of day</param>
Date::Date(ushort year, uchar month, uchar day)
{
    Year = year;
    Month = month;
    Day = day;
}

/// <summary>
/// Check if date is likely to be an existing date.
/// </summary>
/// <returns>true: is an existing date.
/// false: is an non existing date.</returns>
bool Date::isExistingDate()
{
    // The following check will most likely work for another 7000 years at least.
    if ((Month > 0)
        && (Month <= 12)
        && (Year > 0)) {
        if ((Month == _LeapMonth)
            && ((Year % 4) == 0)
            && (((Year % 100) != 0)
                || ((Year % 400)) == 0)) {
            return ((Day > 0) && (Day <= _DaysInLeapMonth));
        }
        else {
            return ((Day > 0) && (Day <= _DaysInMonth[Month]));
        }
    }

    return false;
}
}
}
