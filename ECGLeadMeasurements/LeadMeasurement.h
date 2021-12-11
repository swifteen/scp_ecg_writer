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
    short getValueByIndex(int index);
    MeasurementType getKeyByIndex(int index);
public:
    LeadType leadType ;
    static short NoValue ;
private:
    std::map <int,int> _List;
};
}
}
#endif
