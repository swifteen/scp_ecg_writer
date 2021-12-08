#ifndef _ZQ_SCPSECTION8_H_
#define _ZQ_SCPSECTION8_H_
#include "SCPSection.h"
#include "IDiagnostic.h"    
#include "Statements.h"    
#include "SCPDate.h"
#include "SCPTime.h"

using namespace ECGConversion.ECGDiagnostic;	

namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 8 (contains the full text ECG interpetive statements section).
/// </summary>
class SCPSection8 : public SCPSection,  public IDiagnostic
{
public:
    // getting diagnositc information.
    void getDiagnosticStatements(out Statements stat);
    // setting diagnositc information.
    void setDiagnosticStatements(Statements stat);
protected:
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
    ushort getSectionID();
    bool Works();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    byte _Confirmed ;
    SCPDate _Date ;
    SCPTime _Time ;
    byte _NrStatements ;
    /// <summary>
    /// Class containing SCP diagnostic statement.
    /// </summary>
    class SCPStatement;
    SCPStatement[] _Statements ;
};
}
