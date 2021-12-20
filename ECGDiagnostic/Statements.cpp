#include "Statements.h"


namespace ECGConversion
{
namespace ECGDiagnostic
{
/// <summary>
/// Class containing diagnostic statements.
/// </summary>
Statements::Statements()
{
    confirmed = false;
    interpreted = false;
    //DateTime.MinValue:0001/1/1 0:00:00
    time.Year = 1;
    time.Month = 1;
    time.Day = 1;
    time.Hour = 0;
    time.Minute = 0;
    time.Second = 0;
}
}
}
