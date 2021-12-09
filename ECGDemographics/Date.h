#ifndef _DATE_H_
#define _DATE_H_
#include "ScpGlobal.h"


namespace ECGConversion
{

namespace ECGDemographics
{
/// <summary>
/// Class containing a date (format is equal to SCP).
/// </summary>
class Date
{
	public:
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
	    Date(ushort year, uchar month, uchar day);

	    /// <summary>
	    /// Check if date is likely to be an existing date.
	    /// </summary>
	    /// <returns>true: is an existing date.
	    /// false: is an non existing date.</returns>
	    bool isExistingDate();
	public:

	    // Content of date class.
	    ushort Year ;
	    uchar Month ;
	    uchar Day ;
	public:
	    // Static information for check.
	    static uchar _DaysInMonth[] ;
	    static int _LeapMonth ;
	    static uchar _DaysInLeapMonth ;
};
	
}
}

#endif  /*#ifndef _DATE_H_*/

