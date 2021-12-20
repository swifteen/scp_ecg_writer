#ifndef _IGLOBALMEASUREMENT_CS_
#define _IGLOBALMEASUREMENT_CS_
#include "GlobalMeasurements.h"

namespace ECGConversion
{
namespace ECGGlobalMeasurements
{
/// <summary>
/// Interface for manupalation of the measurements.
/// </summary>
class IGlobalMeasurement
{
public:
#if 0
    /// <summary>
    /// Function to get the global measurements of ECG.
    /// </summary>
    /// <param name="mes">global measurements</param>
    /// <returns>0 on success</returns>
    virtual int getGlobalMeasurements(out GlobalMeasurements mes) = 0;
#endif
    /// <summary>
    /// Function to set the global measurements of ECG.
    /// </summary>
    /// <param name="mes">global measurements</param>
    /// <returns>0 on success</returns>
    virtual int setGlobalMeasurements(GlobalMeasurements& mes) = 0;
};
}
}

#endif  /*#ifndef _IGLOBALMEASUREMENT_CS_*/
