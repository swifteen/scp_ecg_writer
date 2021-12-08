#ifndef _ZQ_SCPSECTION7_H_
#define _ZQ_SCPSECTION7_H_
#include "SCPSection.h"
#include "IGlobalMeasurement.h" 
#include "GlobalMeasurements.h"
using namespace ECGConversion.ECGGlobalMeasurements;

namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 7 (contains the global measurements section).
/// </summary>
class SCPSection7 : public SCPSection, public IGlobalMeasurement
{
    // Defined in SCP.
public:
    ushort getSectionID();
    bool Works();
    int setGlobalMeasurements(GlobalMeasurements mes);
protected:
	int _Write(byte[] buffer, int offset);
	void _Empty();
	int _getLength();
private:
    static ushort _SectionID;

    // Special variables for this section.
    bool _AfterSpikes ;
    bool _AfterSpikesInfo ;
    bool _AfterQRSType ;

    // Part of the stored Data Structure.
    byte _NrRefTypeQRS ;
    byte _NrSpikes;
    ushort _AvgRRInterval ;
    ushort _AvgPPInterval ;
	
    /// <summary>
    /// Class containing measurements for SCP.
    /// </summary>
    class SCPMeasurement;
    SCPMeasurement[] _Measurements;
	
    /// <summary>
    /// Class containing SCP spikes.
    /// </summary>
    class SCPSpike;
    SCPSpike[] _Spikes;
	
    /// <summary>
    /// Class containing SCP Spike info.
    /// </summary>
    class SCPSpikeInfo;
    SCPSpikeInfo[] _SpikesInfo;
	
    ushort _NrQRS ;
    byte[] _QRSType ;
	
    /// <summary>
    /// Class containing SCP extra measurements. (no compatability with UNIPRO at all)
    /// </summary>
    class SCPExtraMeasurements;
    SCPExtraMeasurements _ExtraMeasurements;

    // Manufactor specific block (Not implemented, because UNIPRO doesn't store this kind of info).
    byte[] _Rest ;
};
}
