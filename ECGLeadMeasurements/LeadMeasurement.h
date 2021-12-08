#ifndef _ZQ_LEADMEASUREMENT_H_
#define _ZQ_LEADMEASUREMENT_H_


namespace ECGConversion.ECGLeadMeasurements
{
class LeadMeasurement
{
public:
    LeadMeasurement();
    LeadMeasurement(LeadType lt);
    short getMeasurementType(MeasurementType mt);
    short setMeasurementType(MeasurementType mt);
    int   getMeasurementCount();

    short getValueByIndex(int index);
    MeasurementType getKeyByIndex(int index);
public:
    LeadType LeadType ;
    static short NoValue ;
private:
    SortedList _List;
};
}
