#ifndef _ZQ_QRSZONE_H_
#define _ZQ_QRSZONE_H_


namespace ECGConversion.ECGSignals
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
