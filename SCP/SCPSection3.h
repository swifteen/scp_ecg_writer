#ifndef _SCPSECTION3_H_
#define _SCPSECTION3_H_
#include "SCPSection.h"
#include "Signals.h"
#include "ISignal.h"

using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 3 (Lead definition section).
/// </summary>
class SCPSection3 : public SCPSection, public ISignal
{

public:
    SCPSection3();

    ushort getSectionID();

    bool Works();

    /// <summary>
    /// Function to get number of leads.
    /// </summary>
    /// <returns>number of leads.</returns>
    ushort getNrLeads();
    /// <summary>
    /// Function to get beginning of lead.
    /// </summary>
    /// <param name="nr">number of lead</param>
    /// <returns>begin of lead</returns>
    int getLeadStart(int nr);

    /// <summary>
    /// Function to get lead end.
    /// </summary>
    /// <param name="nr">number of lead</param>
    /// <returns>end of lead</returns>
    int getLeadEnd(int nr);

    /// <summary>
    /// Function to get id of lead.
    /// </summary>
    /// <param name="nr">number of lead</param>
    /// <returns>id of lead</returns>
    int getLeadId(int nr);

    /// <summary>
    /// Function to get length of lead
    /// </summary>
    /// <param name="nr">number of lead</param>
    /// <returns>length of lead</returns>
    int getLeadLength(int nr);

    /// <summary>
    /// Function of minimum beginning of leads.
    /// </summary>
    /// <returns>min begin</returns>
    int getMinBegin();

    /// <summary>
    /// Function of maximum ending of leads
    /// </summary>
    /// <returns>max end</returns>
    int getMaxEnd();

    /// <summary>
    /// Function to get total length of all the leads.
    /// </summary>
    /// <returns>total length</returns>
    int getTotalLength();

    /// <summary>
    /// Function to determine if medians are used.
    /// </summary>
    /// <returns>true if medians are used</returns>
    bool isMediansUsed();

    /// <summary>
    /// Function to set use of median subtraction.
    /// </summary>
    /// <param name="used">true if medians used</param>
    void setMediansUsed(bool used);

    /// <summary>
    /// Function to determine if all leads are simultaneously.
    /// </summary>
    /// <returns>true if leads are simultaneous</returns>
    bool isSimultaneously();

    int setSignals(Signals& signals);

    /// <summary>
    /// Class containing SCP lead information.
    /// </summary>
protected:
    int _Write(uchar* buffer, int bufferLength,int offset);
    void _Empty();
    int _getLength();
private:
    bool _isSimultaneously();
private:
    // Defined in SCP.
    static ushort _SectionID ;

    // Part of the stored Data Structure.
    uchar _NrLeads;
    uchar _Flags ;

    class SCPLead;
    std::vector<SCPLead> _Leads ;
};
}
}
#endif
