#include "LeadMeasurements.h"

namespace ECGConversion
{

namespace ECGLeadMeasurements
{

LeadMeasurements::LeadMeasurements()
{
    //Measurements = null;
}

LeadMeasurements::LeadMeasurements(int nr)
{
    Measurements.resize(nr);
}
}
}
