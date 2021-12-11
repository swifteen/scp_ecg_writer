#include "QRSZone.h"

namespace ECGConversion{

namespace ECGSignals
{
/// <summary>
/// Class containing a QRS zone.
/// </summary>
QRSZone::QRSZone()
{    
    Type = ushort_MaxValue;
    Start = 0;
    Fiducial = 0;
    End = 0;
}

QRSZone::QRSZone(ushort type, int start, int fiducial, int end)
{
    Type = type;
    Start = start;
    Fiducial = fiducial;
    End = end;
}
}

}

