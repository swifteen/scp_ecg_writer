#ifndef _STATEMENTS_H_
#define _STATEMENTS_H_
#include "ScpGlobal.h"

namespace ECGConversion
{

namespace ECGDiagnostic
{

/// <summary>
/// Class containing diagnostic statements.
/// </summary>
class Statements
{
public:
    Statements();
public:
	bool confirmed;
	bool interpreted;
	DateTime time; 
	std::vector<std::string> statement;
};
}
}
#endif /*Statements*/
