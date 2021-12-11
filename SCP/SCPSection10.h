#ifndef _SCPSECTION10_H_
#define _SCPSECTION10_H_
#include "SCPSection.h"
#include "ILeadMeasurement.h"
#include "LeadMeasurements.h"

using namespace ECGConversion::ECGLeadMeasurements;
namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 10 (contains the Lead Measurements Result section).
/// </summary>
class SCPSection10 : public SCPSection, public ILeadMeasurement
{
public:
    SCPSection10();
    ushort getSectionID();
    bool Works();
    //region ILeadMeasurement Members
    void setNrLeads(ushort _NrLeads);
    int setLeadMeasurements(LeadMeasurements& mes);
protected:
    int _Write(uchar* buffer, int bufferLength, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;
    ushort _NrLeads;
    ushort _ManufactorSpecific;
    class SCPLeadMeasurements;
    std::vector<SCPLeadMeasurements> _LeadMeasurements;
};
}
}
#endif  /*#ifndef _SCPSECTION10_H_*/
