#ifndef _SCPSECTION7_H_
#define _SCPSECTION7_H_
#include "SCPSection.h"
#include "IGlobalMeasurement.h" 
#include "GlobalMeasurements.h"
using namespace ECGConversion::ECGGlobalMeasurements;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 7 (contains the global measurements section).
/// </summary>
class SCPSection7 : public SCPSection, public IGlobalMeasurement
{
    // Defined in SCP.
public:
	SCPSection7();
	~SCPSection7();
    ushort getSectionID();
    bool Works();
    int setGlobalMeasurements(GlobalMeasurements& mes);
protected:
	int _Write(uchar* buffer, int bufferLength, int offset);
	void _Empty();
	int _getLength();
private:
    static ushort _SectionID;

    // Special variables for this section.
    bool _AfterSpikes ;
    bool _AfterSpikesInfo ;
    bool _AfterQRSType ;

    // Part of the stored Data Structure.
    uchar _NrRefTypeQRS ;
    uchar _NrSpikes;
    ushort _AvgRRInterval ;
    ushort _AvgPPInterval ;
	
    /// <summary>
    /// Class containing measurements for SCP.
    /// </summary>
    class SCPMeasurement;
    std::vector<SCPMeasurement> _Measurements;
	
    /// <summary>
    /// Class containing SCP spikes.
    /// </summary>
    class SCPSpike;
    std::vector<SCPSpike> _Spikes;
	
    /// <summary>
    /// Class containing SCP Spike info.
    /// </summary>
    class SCPSpikeInfo;
    std::vector<SCPSpikeInfo> _SpikesInfo;
	
    ushort _NrQRS ;
    uchar* _QRSType ;
	
    /// <summary>
    /// Class containing SCP extra measurements. (no compatability with UNIPRO at all)
    /// </summary>
    class SCPExtraMeasurements;
    SCPExtraMeasurements* _ExtraMeasurements;

    // Manufactor specific block (Not implemented, because UNIPRO doesn't store this kind of info).
    uchar* _Rest ;
};
}
}
#endif
