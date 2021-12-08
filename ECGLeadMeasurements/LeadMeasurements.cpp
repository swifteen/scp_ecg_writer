#include "LeadMeasurements.h"

namespace ECGConversion.ECGLeadMeasurements
{

LeadMeasurements::LeadMeasurements()
{
	Measurements = null;
}

LeadMeasurements::LeadMeasurements(int nr)
{
    Measurements = new LeadMeasurement[nr];

    for (int i=0;i < Measurements.Length;i++)
        Measurements[i] = new LeadMeasurement();
}
}
