#ifndef _GLOBALMEASUREMENT_H_
#define _GLOBALMEASUREMENT_H_
#include  "ScpGlobal.h"

namespace ECGConversion
{
 namespace	ECGGlobalMeasurements
 	{

/// <summary>
/// Class containing one wave measurement (SCP an UNIPRO defined).
/// </summary>
	class GlobalMeasurement
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
	    GlobalMeasurement();
	    ushort getPdur();
	    void setPdur(ushort Pdur);
	    ushort getPRint();
	    void setPRint(ushort PRint);
	    ushort getQRSdur();
	    void setQRSdur(ushort QRSdur);
		ushort getTdur();				
		void setTdur(ushort Tdur);
	    ushort getQTdur();
	    void setQTdur(ushort QTdur);
	    ushort calcQTc(ushort AvgRR, ushort HR, QTcCalcType calcType);
	public:
		static ushort NoValue;
		static short NoAxisValue;
		ushort Ponset;
		ushort Poffset;
		ushort QRSonset;
		ushort QRSoffset;
		ushort Toffset;
		short Paxis;
		short QRSaxis;
		short Taxis;
	};
 	}
}
#endif
