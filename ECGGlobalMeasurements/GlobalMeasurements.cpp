#include "GlobalMeasurements.h"


namespace ECGConversion
{

namespace ECGGlobalMeasurements
{
/// <summary>
/// Class containing measurements of ECG.
/// </summary>
GlobalMeasurements::GlobalMeasurements()
{
    _QTc = GlobalMeasurement::NoValue;
    _VentRate = GlobalMeasurement::NoValue;
    AvgRR = GlobalMeasurement::NoValue;
    AvgPP = GlobalMeasurement::NoValue;
  //  measurment = null;
 //   spike = null;
}

void GlobalMeasurements::setVentRate(ushort VentRate)
{
    _VentRate = VentRate < GlobalMeasurement::NoValue ? VentRate : GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setPdur(ushort Pdur)
{



    if (measurment.size() == 0)
    {

		measurment.resize(1);
        //measurment = new GlobalMeasurement[1];
        measurment[0] = new GlobalMeasurement();
    }

    measurment[0]->setPdur(Pdur);
}

void GlobalMeasurements::setPRint(ushort PRint)
{
  
	if (measurment.size() == 0)
    {
        measurment.resize(1);
        //measurment = new GlobalMeasurement[1];
        measurment[0] = new GlobalMeasurement();
    }

    measurment[0]->setPRint(PRint);
    
}

void GlobalMeasurements::setQRSdur(ushort QRSdur)
{
  
    if (measurment.size() == 0)
    {
       measurment.resize(1);
        //measurment = new GlobalMeasurement[1];
        measurment[0] = new GlobalMeasurement();
    }

    measurment[0]->setQRSdur(QRSdur);
    
}

void GlobalMeasurements::setQTdur(ushort QTdur)
{


    if (measurment.size() == 0)
    {
       measurment.resize(1);
        //measurment = new GlobalMeasurement[1];
        measurment[0] = new GlobalMeasurement();
    }

    measurment[0]->setQTdur(QTdur);
    
}
void GlobalMeasurements::setQTc(ushort QTc)
{
    _QTc = QTc;
}
void GlobalMeasurements::setQTcType(ushort QTcType)
{
   
	if (QTcType != Unknown)
        _QTc = (ushort) (GlobalMeasurement::NoValue + QTcType);
    else if (_QTc >= GlobalMeasurement::NoValue)
        _QTc = 0;
        
}

GlobalMeasurements GlobalMeasurements::Clone()
{

GlobalMeasurements ret;

/* TODO
	GlobalMeasurements ret = new GlobalMeasurements();

    ret._QTc = _QTc;
    ret._VentRate = VentRate;
    ret.AvgPP = AvgPP;
    ret.AvgRR = AvgRR;

    if (measurment != null)
    {
        ret.measurment = new GlobalMeasurement[measurment.Length];
        for (int i=0;i < measurment.Length;i++)
        {
            ret.measurment[i] = new GlobalMeasurement();

            ret.measurment[i].Ponset = measurment[i].Ponset;
            ret.measurment[i].Poffset = measurment[i].Poffset;
            ret.measurment[i].QRSonset = measurment[i].QRSonset;
            ret.measurment[i].QRSoffset = measurment[i].QRSoffset;
            ret.measurment[i].Toffset = measurment[i].Toffset;

            ret.measurment[i].Paxis = measurment[i].Paxis;
            ret.measurment[i].QRSaxis = measurment[i].QRSaxis;
            ret.measurment[i].Taxis = measurment[i].Taxis;
        }
    }

    if (spike != null)
    {
        ret.spike = new Spike[spike.Length];
        for (int i=0;i < spike.Length;i++)
        {
            ret.spike[i] = new Spike();
            ret.spike[i].Amplitude = spike[i].Amplitude;
            ret.spike[i].Time = spike[i].Time;
        }
    }
*/
    return ret;
}
}
}
