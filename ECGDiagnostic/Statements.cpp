#include "Statements.h"


namespace ECGConversion.ECGDiagnostic
{
/// <summary>
/// Class containing diagnostic statements.
/// </summary>

Statements::Statements()
{
    confirmed = false;
    interpreted = false;
    time = DateTime.MinValue;
    statement = null;
}

}
