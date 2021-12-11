#include "LeadMeasurement.h"


using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{

  namespace ECGLeadMeasurements
  	{


short LeadMeasurement::NoValue = 29999;

LeadMeasurement::LeadMeasurement()
{
   // leadType = LeadType::Unknown; TODO
    //_List = new SortedList();
}

LeadMeasurement::LeadMeasurement(LeadType lt)
{
    leadType = lt;
}

short LeadMeasurement::getMeasurementType(MeasurementType mt)
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
   getValue=index >= 0 ? (short) _List.at(index) : NoValue;
   // int index = _List.IndexOfKey((int) mt);
    return  getValue;
    
}

void LeadMeasurement::setMeasurementType(MeasurementType mt)
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


//    int index = _List.IndexOfKey((int) mt);

    if (getValue == NoValue)
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
			_List[index] =getValue;
           // _List.SetByIndex(index, value);
        else
			_List[mt] =getValue;
            //_List.insert((int) mt, (int)getValue);
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
