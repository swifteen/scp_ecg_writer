#ifndef _GLOBALMEASUREMENTS_H_
#define _GLOBALMEASUREMENTS_H_
#include <vector>
#include "Spike.h"
#include "GlobalMeasurement.h"
namespace ECGConversion
{

namespace ECGGlobalMeasurements
{

/// <summary>
/// Class containing measurements of ECG.
/// </summary>
class GlobalMeasurements
{
public:
    GlobalMeasurements();
    GlobalMeasurements Clone();

    void setVentRate(ushort VentRate);
    ushort getVentRate();
    void setPdur(ushort Pdur);
    ushort getPdur();
    void setPRint(ushort PRint);
    ushort getPRint();
    void setQRSdur(ushort QRSdur);
    ushort getQRSdur();
    void setQTdur(ushort QTdur);
    ushort getQTdur();
    void setAxis(short Paxis, short QRSaxis, short Taxis);
    void setQTc(ushort QTc);
    ushort getQTc();
    void setQTcType(GlobalMeasurement::QTcCalcType QTcType);
    GlobalMeasurement::QTcCalcType getQTcType();
public:
    ushort AvgRR;
    ushort AvgPP;
    std::vector<GlobalMeasurement> measurment;
    std::vector<Spike> spike;
private:
    ushort _QTc;
    ushort _VentRate;
};
}
}
#endif
