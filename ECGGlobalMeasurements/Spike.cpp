#include "Spike.h"

#include "GlobalMeasurement.h"
namespace ECGConversion
{

namespace ECGGlobalMeasurements
{

/// <summary>
/// Class containing one spike (SCP defined).
/// </summary>
Spike::Spike()
{
    Time = GlobalMeasurement::NoValue;
    Amplitude = GlobalMeasurement::NoAxisValue;
}
}
}
