#ifndef _LEADMEASUREMENTS_H_
#define _LEADMEASUREMENTS_H_
#include "LeadMeasurement.h"

namespace ECGConversion.ECGLeadMeasurements
{
class LeadMeasurements
{
public:
    LeadMeasurements();
    LeadMeasurements(int nr);
public:
    LeadMeasurement[] Measurements;
};
}
