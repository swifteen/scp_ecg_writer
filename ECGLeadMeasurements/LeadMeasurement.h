#ifndef _LEADMEASUREMENT_H_
#define _LEADMEASUREMENT_H_
#include "MeasurementType.h"
#include "LeadType.h"
#include <vector>
#include <map>

using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{
namespace ECGLeadMeasurements
{
class LeadMeasurement
{
public:
    LeadMeasurement();
    LeadMeasurement(LeadType lt);
	short getMeasurement(MeasurementType mt);	
	void setMeasurement(MeasurementType mt,short measurementValue);
    int getMeasurementCount();
	bool getMeasurementValid(MeasurementType mt);
public:
    LeadType leadType ;
    static short NoValue ;
private:
    short measurementValueArray[MeasurementTypeSum];	
	bool measurementExistArray[MeasurementTypeSum];	
};
}
}
#endif
