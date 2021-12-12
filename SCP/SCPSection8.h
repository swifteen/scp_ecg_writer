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
    // getting diagnositc information.
    int getDiagnosticStatements(Statements stat);
    // setting diagnositc information.
    int setDiagnosticStatements(Statements stat);
    bool Works();
protected:
    int _Write(uchar buffer[], int bufferLength,int offset);
    void _Empty();
    int _getLength();
    ushort getSectionID();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    uchar _Confirmed ;


	SCPDate _Date;
	SCPTime _Time;
    //SCPDate _Date ;
   // SCPTime _Time ;
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
