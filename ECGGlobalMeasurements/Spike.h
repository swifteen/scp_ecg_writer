#ifndef _SPIKE_H_
#define _SPIKE_H_
#include  "ScpGlobal.h"

namespace ECGConversion
{

namespace ECGGlobalMeasurements
{

/// <summary>
/// Class containing one spike (SCP defined).
/// </summary>
class Spike
{
public:
    Spike();
public:
    ushort Time;
    short Amplitude;
};
}
}
#endif
