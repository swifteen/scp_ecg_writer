#ifndef _SCPSECTION11_H_
#define _SCPSECTION11_H_
#include "SCPSection.h"
#include "SCPDate.h"
#include "SCPTime.h"

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 11 (contains the Universal ECG interpretive statements section).
/// </summary>
class SCPSection11 : public SCPSection
{
public:
    SCPSection11();
    ushort getSectionID();
    bool Works();
protected:
    int _Write(uchar* buffer, int bufferLength, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    uchar _Confirmed;
    SCPDate _Date;
    SCPTime _Time;
    uchar _NrStatements;
    class SCPStatement;
    std::vector<SCPStatement> _Statements;
};
}
}
#endif  /*#ifndef _SCPSECTION11_H_*/
