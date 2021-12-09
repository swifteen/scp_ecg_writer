#ifndef _SCPSECTION11_H_
#define _SCPSECTION11_H_
#include "SCPSection.h"

namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 11 (contains the Universal ECG interpretive statements section).
/// </summary>
class SCPSection11 : public SCPSection
{
public:
    ushort getSectionID();
    bool Works();
protected:
    int _Read(byte[] buffer, int offset);
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    byte _Confirmed;
    SCPDate _Date;
    SCPTime _Time;
    byte _NrStatements;
    class SCPStatement;
    SCPStatement[] _Statements;
};
}

#endif  /*#ifndef _SCPSECTION11_H_*/
