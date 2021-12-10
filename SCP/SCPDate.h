#ifndef _SCPDATE_H_
#define _SCPDATE_H_
#include "ScpGlobal.h"

namespace ECGConversion
{
namespace SCP
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


    SCPDate& operator=(const SCPDate& date)
    {
        // Prevent self-assignment
        if( &date != this )
        {
			 this->Year=0;
			 this->Month=0;
			 this->Day  =0;
			deepCopy(date);
        }
        return *this;
    }
    void deepCopy(const SCPDate& date)
    {
	     this->Year  = date.Year;
		 this->Month = date.Month;
		 this->Day   = date.Day;
	}
	
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
    int Write(uchar* buffer, int bufferLength, int offset);
public:
    const int Size;
    // data structure of SCP date.
    ushort Year;
    uchar Month;
    uchar Day;
};
}
}
#endif
