#ifndef _ZQ_SCPSECTION10_H_
#define _ZQ_SCPSECTION10_H_
#include "SCPSection.h"
#include "ILeadMeasurement.h"

namespace ECGConversion.SCP
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
    int getLeadMeasurements(out LeadMeasurements mes);
    int setLeadMeasurements(LeadMeasurements mes);
protected:
    int _Read(byte[] buffer, int offset);
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    void setLeadMeasurements(ushort _NrLeads);
private:
    // Defined in SCP.
    static ushort _SectionID;
    ushort _ManufactorSpecific;
    class SCPLeadMeasurements;
    SCPLeadMeasurements[] _LeadMeasurements;
};
}

#endif  /*#ifndef _ZQ_SCPSECTION10_H_*/
