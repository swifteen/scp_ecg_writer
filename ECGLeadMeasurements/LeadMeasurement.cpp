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
    _List.clear();
}

LeadMeasurement::LeadMeasurement(LeadType lt)
{
    leadType = lt;
    _List.clear();
}

short LeadMeasurement::getMeasurement(MeasurementType mt)
{
    int index =-1;

    for(int i=0;i<_List.size();i++)
    {
        if(_List.at(i)==mt)
        {
            index =i;
            break;
        }
    }
    return  (index >= 0 ? (short) _List.at(index) : NoValue);  
}

void LeadMeasurement::setMeasurement(MeasurementType mt,short measurementValue)
{
    std::map<int,int>::iterator  it;

    int index =-1;

    for(int i=0;i<_List.size();i++)
    {
        if(_List.at(i)==mt)
        {
            index =i;
            break;
        }
    }

    if (measurementValue == NoValue)
    {
        if (index >= 0)
        {
            it=_List.find(index);
            _List.erase(it);
        }
    }
    else
    {
        if (index >= 0)
            _List[index] = measurementValue;
        else
            _List[mt] = measurementValue;
    }
}

int LeadMeasurement::getMeasurementCount()
{
    return _List.size();
}

short LeadMeasurement::getValueByIndex(int index)
{
    return (index >= 0) && (index < _List.size()) ? (short) _List.at(index) : NoValue;
}

MeasurementType  LeadMeasurement::getKeyByIndex(int index)
{
    return (index >= 0) && (index < _List.size()) ? (MeasurementType) ((int) _List.at(index)) :(MeasurementType) -1;
}


}
}
