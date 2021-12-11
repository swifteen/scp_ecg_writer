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
	    enum QTcCalcType
	    {
	        Unknown = -1,
	        Bazett = 0,
	        Hodges = 1,
	        Fridericia = 2,
	        Framingham = 3,

	    };
	public:
	    GlobalMeasurements();
	    GlobalMeasurements Clone();

	    void setVentRate(ushort VentRate);
	    void setPdur(ushort Pdur);
	    void setPRint(ushort PRint);
	    void setQRSdur(ushort QRSdur);
	    void setQTdur(ushort QTdur);
	    void setQTc(ushort QTc);
	    void setQTcType(ushort QTcType);
	public:
	    std::vector<GlobalMeasurement> measurment;
	    std::vector<Spike> spike;
	private:
	    ushort _QTc;
	    ushort _VentRate;
	    ushort AvgRR;
	    ushort AvgPP;
	};
 }
}
#endif
