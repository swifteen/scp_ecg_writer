#include "LeadMeasurement.h"

namespace ECGConversion.ECGLeadMeasurements
{

short LeadMeasurement::NoValue = 29999;

LeadMeasurement::LeadMeasurement()
{
    LeadType = LeadType.Unknown;
    _List = new SortedList();
}

LeadMeasurement::LeadMeasurement(LeadType lt)
{
    LeadType = lt;
}

short LeadMeasurement::getMeasurementType(MeasurementType mt)
{
    int index = _List.IndexOfKey((int) mt);
    return index >= 0 ? (short) _List.GetByIndex(index) : NoValue;
}

short LeadMeasurement::setMeasurementType(MeasurementType mt)
{
    int index = _List.IndexOfKey((int) mt);

    if (value == NoValue)
    {
        if (index >= 0)
            _List.RemoveAt(index);
    }
    else
    {
        if (index >= 0)
            _List.SetByIndex(index, value);
        else
            _List.Add((int) mt, value);
    }
}

int LeadMeasurement::getMeasurementCount()
{
    return _List.Count;
}

short LeadMeasurement::getValueByIndex(int index)
{
    return (index >= 0) && (index < _List.Count) ? (short) _List.GetByIndex(index) : NoValue;
}

MeasurementType  LeadMeasurement::getKeyByIndex(int index)
{
    return (index >= 0) && (index < _List.Count) ? (MeasurementType) ((int) _List.GetKey(index)) : MeasurementType.None;
}

}
