#ifndef _QRSZONE_H_
#define _QRSZONE_H_

#include "ScpGlobal.h"

namespace ECGConversion
{

namespace ECGSignals
{
/// <summary>
/// Class containing a QRS zone.
/// </summary>
class QRSZone
{
public:
    QRSZone();
    QRSZone(ushort type, int start, int fiducial, int end);
public:
    ushort Type;
    int Start;
    int Fiducial;
    int End;
};

}
}

#endif
