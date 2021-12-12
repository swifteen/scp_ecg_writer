#ifndef _SCPSECTION8_H_
#define _SCPSECTION8_H_
#include "SCPSection.h"
#include "IDiagnostic.h"    
#include "Statements.h"    
#include "SCPDate.h"
#include "SCPTime.h"
#include <vector>
#include <string>


using namespace ECGConversion::ECGDiagnostic;	

namespace ECGConversion
{

namespace SCP
{
/// <summary>
/// Class contains section 8 (contains the full text ECG interpetive statements section).
/// </summary>
class SCPSection8 : public SCPSection,  public IDiagnostic
{
public:
	 SCPSection8();
    // setting diagnositc information.
    int setDiagnosticStatements(Statements& stat);
    bool Works();
    ushort getSectionID();
protected:
    int _Write(uchar* buffer, int bufferLength,int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    uchar _Confirmed ;

	SCPDate _Date;
	SCPTime _Time;
    uchar _NrStatements ;
    /// <summary>
    /// Class containing SCP diagnostic statement.
    /// </summary>
    class SCPStatement;
   // SCPStatement *_Statements;
	std::vector<SCPStatement> _Statements;
};
}
}
#endif 
