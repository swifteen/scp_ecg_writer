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

	    for (int i=0;i < nr;i++)
	        Measurements[i] = new LeadMeasurement();
	}
 }
}
