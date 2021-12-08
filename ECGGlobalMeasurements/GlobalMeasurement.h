#ifndef _ZQ_GLOBALMEASUREMENT_H_
#define _ZQ_GLOBALMEASUREMENT_H_

namespace ECGConversion.ECGGlobalMeasurements
{
/// <summary>
/// Class containing one wave measurement (SCP an UNIPRO defined).
/// </summary>
class GlobalMeasurement
{
public:
    GlobalMeasurement();

    ushort Pdur_get();
    ushort Pdur_set(ushort value);

    ushort PRint_get();
    ushort PRint_set(ushort value);

    ushort QRSdur_get();
    ushort QRSdur_set(ushort value);

    ushort Tdur_get();

    ushort QTdur_get();
    ushort QTdur_set(ushort value);

    ushort calcQTc(ushort AvgRR, ushort HR, GlobalMeasurements::QTcCalcType calcType);
public:
	static ushort NoValue ;
	static short NoAxisValue;
	ushort Ponset;
	ushort Poffset;
	ushort QRSonset;
	ushort QRSoffset;
	ushort Toffset ;
	short Paxis ;
	short QRSaxis ;
	short Taxis ;
};

}
