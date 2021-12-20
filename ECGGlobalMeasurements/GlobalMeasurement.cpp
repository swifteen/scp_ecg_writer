#include "GlobalMeasurement.h"
#include <math.h>

namespace ECGConversion
{

namespace ECGGlobalMeasurements
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

ushort GlobalMeasurement::getPdur()
{
    return (ushort)((Poffset != NoValue)
                    && (Ponset != NoValue)
                    && (Ponset < Poffset) ? (Poffset - Ponset) : NoValue);
}

void GlobalMeasurement::setPdur(ushort Pdur)
{
    if ((Pdur > 0)
        && (Pdur != NoValue)) {
        if (Ponset == NoValue) {
            Ponset = 100;
        }

        Poffset = (ushort)(Pdur + Ponset);
    }
    else {
        Ponset = NoValue;
        Poffset = NoValue;
    }
}

ushort GlobalMeasurement::getPRint()
{
    return (ushort)((QRSonset != NoValue) && (Ponset != NoValue) ? (QRSonset - Ponset) : NoValue);
}

void GlobalMeasurement::setPRint(ushort PRint)
{
    if ((PRint > 0)
        && (PRint != NoValue)) {
        if (Ponset == NoValue) {
            Ponset = 100;
            Poffset = NoValue;
        }

        QRSonset = (ushort)(PRint + Ponset);
    }
}

ushort GlobalMeasurement::getQRSdur()
{
    return (ushort)((QRSoffset != NoValue) && (QRSonset != NoValue) ? (QRSoffset - QRSonset) : NoValue);
}

void GlobalMeasurement::setQRSdur(ushort QRSdur)
{
    if ((QRSdur != NoValue)
        && (QRSdur != 0)) {
        if ((QRSonset == NoValue)
            || (QRSonset == 0)) {
            Ponset = NoValue;
            Poffset = NoValue;
            QRSonset = 400;
        }

        QRSoffset = (ushort)(QRSdur + QRSonset);
    }
}

ushort GlobalMeasurement::getTdur()
{
    return (ushort)((Toffset != NoValue) && (Ponset != NoValue) ? (Toffset - QRSoffset) : NoValue);
}

void GlobalMeasurement::setTdur(ushort Tdur)
{
#if 0

    if ((value != NoValue)
        && (value != 0)) {
        if ((Toffset != NoValue)
            && (Toffset != 0)) {
            Tonset = Toffset - value;
        }
        else {
            throw new Exception("You should set QTdur(ation) or Toffset before setting Tdur(ation)! ");
        }
    }
    else {
        Tonset = NoValue;
        Toffset = NoValue;
    }

#endif
}

ushort GlobalMeasurement::getQTdur()
{
    return (ushort)((Toffset != NoValue) && (QRSonset != NoValue) ? (Toffset - QRSonset) : NoValue);
}

void GlobalMeasurement::setQTdur(ushort QTdur)
{
    if ((QTdur != NoValue)
        && (QTdur != 0)
        && (QRSonset != NoValue)
        && (QRSonset != 0)) {
        Toffset = (ushort)(QRSonset + QTdur);
    }
    else {
        Toffset = NoValue;
    }
}
void GlobalMeasurement::setAxis(short Paxis, short QRSaxis, short Taxis)
{
    this->Paxis = Paxis;
    this->QRSaxis = QRSaxis;
    this->Taxis = Taxis;
}

ushort GlobalMeasurement::calcQTc(ushort AvgRR, ushort HR, QTcCalcType calcType)
{
    if ((AvgRR == 0)
        || (AvgRR == NoValue)
        || (getQTdur() == NoValue)) {
        return NoValue;
    }

    ushort ret = NoValue;

    switch (calcType) {
        case QTcCalcTypeBazett:
            ret = (ushort)(getQTdur() / sqrt(AvgRR * 0.001));
            break;

        case QTcCalcTypeFridericia:
            ret = (ushort)(getQTdur() / pow(AvgRR * 0.001, 1.0 / 3.0));
            break;

        case QTcCalcTypeFramingham:
            ret = (ushort)(getQTdur() + (154 * (1 - (AvgRR * 0.001))));
            break;

        case QTcCalcTypeHodges:
            ret = (ushort)(getQTdur() + (1.75 * (HR - 60)));
            break;

        default:
            break;
    }

    return ret;
}
}
}
