#include "GlobalMeasurement.h"

namespace ECGConversion.ECGGlobalMeasurements
{
/// <summary>
/// Class containing one wave measurement (SCP an UNIPRO defined).
/// </summary>

ushort GlobalMeasurement::NoValue = 29999;
short  GlobalMeasurement::NoAxisValue = 29999;

GlobalMeasurement::GlobalMeasurement()
{
    Ponset = NoValue;
    Poffset = NoValue;
    QRSonset = NoValue;
    QRSoffset = NoValue;
    Toffset = NoValue;
    Paxis = NoAxisValue;
    QRSaxis = NoAxisValue;
    Taxis = NoAxisValue;
}

ushort GlobalMeasurement::Pdur_get()
{
    return (ushort) ((Poffset != NoValue) && (Ponset != NoValue) && (Ponset < Poffset) ? (Poffset - Ponset) : NoValue);
}

ushort GlobalMeasurement::Pdur_set(ushort value)
{
    if ((value > 0)
            &&	(value != NoValue))
    {
        if (Ponset == NoValue)
            Ponset = 100;

        Poffset = (ushort) (value + Ponset);
    }
    else
    {
        Ponset = NoValue;
        Poffset = NoValue;
    }
}

ushort GlobalMeasurement::PRint_get()
{
    return (ushort) ((QRSonset != NoValue) && (Ponset != NoValue) ? (QRSonset - Ponset) : NoValue);
}

ushort GlobalMeasurement::PRint_set(ushort value)
{
    if ((value > 0)
            &&	(value != NoValue))
    {
        if (Ponset == NoValue)
        {
            Ponset = 100;
            Poffset = NoValue;
        }

        QRSonset = (ushort) (value + Ponset);
    }
}

ushort GlobalMeasurement::QRSdur_get()
{
    return (ushort) ((QRSoffset != NoValue) && (QRSonset != NoValue) ? (QRSoffset - QRSonset) : NoValue);
}

ushort GlobalMeasurement::QRSdur_set(ushort value)
{
    if ((value != NoValue)
            &&	(value != 0))
    {
        if ((QRSonset == NoValue)
                ||	(QRSonset == 0))
        {
            Ponset = NoValue;
            Poffset = NoValue;

            QRSonset = 400;
        }

        QRSoffset = (ushort) (value + QRSonset);
    }
}

ushort GlobalMeasurement::Tdur_get()
{
    return (ushort) ((Toffset != NoValue) && (Ponset != NoValue) ? (Toffset - QRSoffset) : NoValue);
}

ushort GlobalMeasurement::QTdur_get()
{
    return (ushort) ((Toffset != NoValue) && (QRSonset != NoValue) ? (Toffset - QRSonset) : NoValue);
}

ushort GlobalMeasurement::QTdur_set(ushort value)
{
    if ((value != NoValue)
            &&	(value != 0)
            &&	(QRSonset != NoValue)
            &&	(QRSonset != 0))
    {
        Toffset = (ushort) (QRSonset + value);
    }
    else
    {
        Toffset = NoValue;
    }
}

ushort GlobalMeasurement::calcQTc(ushort AvgRR, ushort HR, GlobalMeasurements::QTcCalcType calcType)
{
    if ((AvgRR == 0)
            ||	(AvgRR == NoValue)
            ||	(QTdur == NoValue))
        return NoValue;

    ushort ret = NoValue;

    switch (calcType)
    {
    case GlobalMeasurements::QTcCalcType::Bazett:
        ret = (ushort) (QTdur / Math.Sqrt(AvgRR * 0.001));
        break;
    case GlobalMeasurements::QTcCalcType::Fridericia:
        ret = (ushort) (QTdur / Math.Pow(AvgRR * 0.001, 1.0/3.0));
        break;
    case GlobalMeasurements::QTcCalcType::Framingham:
        ret = (ushort) (QTdur + (154 * (1 - (AvgRR * 0.001))));
        break;
    case GlobalMeasurements::QTcCalcType::Hodges:
        ret = (ushort) (QTdur + (1.75 * (HR - 60)));
        break;
    default:break;
    }

    return ret;
}

}
