#ifndef _LEADMEASUREMENTS_H_
#define _LEADMEASUREMENTS_H_
#include "LeadMeasurement.h"
#include <vector>



namespace ECGConversion
{

namespace ECGLeadMeasurements
{

class LeadMeasurements
{
public:
    LeadMeasurements();
    LeadMeasurements(int nr);
public:
    std::vector<LeadMeasurement> Measurements;
};


}
}
#endif 
