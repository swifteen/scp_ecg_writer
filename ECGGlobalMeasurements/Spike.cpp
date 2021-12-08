#include "Spike.h"


namespace ECGConversion.ECGGlobalMeasurements
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
