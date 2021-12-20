#include "LeadMeasurement.h"


using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{
namespace ECGLeadMeasurements
{


short LeadMeasurement::NoValue = 29999;

LeadMeasurement::LeadMeasurement()
{
    leadType = LeadTypeUnknown;

    for (int i = 0; i < MeasurementTypeSum; i++) {
        measurementExistArray[i] = false;
    }
}

LeadMeasurement::LeadMeasurement(LeadType lt)
{
    leadType = lt;

    for (int i = 0; i < MeasurementTypeSum; i++) {
        measurementExistArray[i] = false;
    }
}

short LeadMeasurement::getMeasurement(MeasurementType mt)
{
    if ((mt < MeasurementTypeSum) && (measurementExistArray[(int)mt])) {
        return measurementValueArray[(int)mt];
    }

    return NoValue;
}

void LeadMeasurement::setMeasurement(MeasurementType mt, short measurementValue)
{
    if ((MeasurementTypeNone < mt) && (mt < MeasurementTypeSum)) {
        if (measurementValue == NoValue) {
            measurementExistArray[(int)mt] = false;
        }
        else {
            measurementExistArray[(int)mt] = true;
            measurementValueArray[(int)mt] = measurementValue;
        }
    }
}

int LeadMeasurement::getMeasurementCount()
{
    int count = 0;

    for (int i = 0; i < MeasurementTypeSum; i++) {
        if (measurementExistArray[i]) {
            return count++;
        }
    }

    return count;
}

bool LeadMeasurement::getMeasurementValid(MeasurementType mt)
{
    if (mt < MeasurementTypeSum) {
        return measurementExistArray[(int)mt];
    }

    return false;
}

}
}
