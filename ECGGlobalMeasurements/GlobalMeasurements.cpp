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
    measurment.clear();
    spike.clear();
}

void GlobalMeasurements::setVentRate(ushort VentRate)
{
    _VentRate = VentRate < GlobalMeasurement::NoValue ? VentRate : GlobalMeasurement::NoValue;
}

ushort GlobalMeasurements::getVentRate()
{
    if (_VentRate < GlobalMeasurement::NoValue) {
        return _VentRate;
    }

    return (ushort)((AvgRR == 0) || (AvgRR == GlobalMeasurement::NoValue) ? 0 : (60000 / AvgRR));
}

void GlobalMeasurements::setPdur(ushort Pdur)
{
    if (measurment.size() == 0) {
        measurment.resize(1);
    }

    measurment[0].setPdur(Pdur);
}

ushort GlobalMeasurements::getPdur()
{
    if (measurment.size() > 0) {
        return measurment[0].getPdur();
    }

    return GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setPRint(ushort PRint)
{
    if (measurment.size() == 0) {
        measurment.resize(1);
    }

    measurment[0].setPRint(PRint);
}

ushort GlobalMeasurements::getPRint()
{
    if (measurment.size() > 0) {
        return measurment[0].getPRint();
    }

    return GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setQRSdur(ushort QRSdur)
{
    if (measurment.size() == 0) {
        measurment.resize(1);
    }

    measurment[0].setQRSdur(QRSdur);
}

ushort GlobalMeasurements::getQRSdur()
{
    if (measurment.size() > 0) {
        return measurment[0].getQRSdur();
    }

    return GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setQTdur(ushort QTdur)
{
    if (measurment.size() == 0) {
        measurment.resize(1);
    }

    measurment[0].setQTdur(QTdur);
}

ushort GlobalMeasurements::getQTdur()
{
    if (measurment.size() > 0) {
        return measurment[0].getQTdur();
    }

    return GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setQTc(ushort QTc)
{
    _QTc = QTc;
}

ushort GlobalMeasurements::getQTc()
{
    if (_QTc < GlobalMeasurement::NoValue) {
        return _QTc;
    }

    if ((measurment.size() > 0)
        && (AvgRR != GlobalMeasurement::NoValue)) {
        return measurment[0].calcQTc(AvgRR, getVentRate(), getQTcType());
    }

    return GlobalMeasurement::NoValue;
}

void GlobalMeasurements::setQTcType(GlobalMeasurement::QTcCalcType QTcType)
{
    if (QTcType != GlobalMeasurement::QTcCalcTypeUnknown) {
        _QTc = (ushort)(GlobalMeasurement::NoValue + QTcType);
    }
    else if (_QTc >= GlobalMeasurement::NoValue) {
        _QTc = 0;
    }
}

GlobalMeasurement::QTcCalcType GlobalMeasurements::getQTcType()
{
    if (_QTc >= GlobalMeasurement::NoValue) {
        return (GlobalMeasurement::QTcCalcType)(_QTc - GlobalMeasurement::NoValue);
    }

    return GlobalMeasurement::QTcCalcTypeUnknown;
}

GlobalMeasurements GlobalMeasurements::Clone()
{
    GlobalMeasurements ret ;
    ret._QTc = _QTc;
    ret._VentRate = _VentRate;
    ret.AvgPP = AvgPP;
    ret.AvgRR = AvgRR;

    if (measurment.size() != 0) {
        //  ret.measurment = new GlobalMeasurement[measurment.Length];
        ret.measurment.resize(measurment.size());

        for (int i = 0; i < measurment.size(); i++) {
            //ret.measurment[i] = new GlobalMeasurement();
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

    if (spike.size() != 0) {
        // ret.spike = new Spike[spike.Length];
        ret.spike.resize(spike.size());

        for (int i = 0; i < spike.size(); i++) {
            //  ret.spike[i] = new Spike();
            ret.spike[i].Amplitude = spike[i].Amplitude;
            ret.spike[i].Time = spike[i].Time;
        }
    }

    return ret;
}
}
}
