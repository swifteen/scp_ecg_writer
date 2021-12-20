#include "SCPSection4.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGSignals;
using namespace ECGConversion::ECGGlobalMeasurements;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class containing SCP QRS subtraction zone.
/// </summary>
class SCPSection4::SCPQRSSubtraction
{
public:
    /// <summary>
    /// Constructor for an QRS Subtraction zone.
    /// </summary>
    SCPQRSSubtraction()
    {
        ushort Type = ushort_MaxValue;
        int Start = 0;
        int Fiducial = 0;
        int End = 0;
    }
    /// <summary>
    /// Constructor for an QRS Subtraction zone.
    /// </summary>
    /// <param name="type">type of subtraction</param>
    /// <param name="start">starting point of subtraction zone</param>
    /// <param name="fiducial">fiducial point in subtraction zone</param>
    /// <param name="end">ending point of subtraction zone</param>
    SCPQRSSubtraction(uchar type, int start, int fiducial, int end)
    {
        Type = type;
        Start = start;
        Fiducial = fiducial;
        End = end;
    }

    /// <summary>
    /// Function to write QRS Subtraction zone.
    /// </summary>
    /// <param name="buffer">byte array</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if (offset + Size > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Type, buffer, bufferLength, offset, sizeof(Type), true);
        offset += sizeof(Type);
        BytesTool::writeBytes(Start, buffer, bufferLength, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(Fiducial, buffer, bufferLength, offset, sizeof(Fiducial), true);
        offset += sizeof(Fiducial);
        BytesTool::writeBytes(End, buffer, bufferLength, offset, sizeof(End), true);
        offset += sizeof(End);
        return 0x0;
    }
public:
    static int Size;
    ushort Type;
    int Start;
    int Fiducial;
    int End;
};

int SCPSection4::SCPQRSSubtraction::Size = 14;

/// <summary>
/// Class containing QRS protected zones.
/// </summary>
class SCPSection4::SCPQRSProtected
{
public:
    /// <summary>
    /// Constructor to create an QRS protected zone.
    /// </summary>
    SCPQRSProtected()
    {
        Start = 0;
        End = 0;
    }
    /// <summary>
    /// Constructor to create an QRS protected zone.
    /// </summary>
    /// <param name="start">start sample of zone</param>
    /// <param name="end">end sample of zone</param>
    SCPQRSProtected(int start, int end)
    {
        Start = start;
        End = end;
    }

    /// <summary>
    /// Function to write QRS protected zone.
    /// </summary>
    /// <param name="buffer">byte array to write to</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if (offset + Size > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Start, buffer, bufferLength, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(End, buffer, bufferLength, offset, sizeof(End), true);
        offset += sizeof(End);
        return 0x0;
    }
public:
    static int Size;

    int Start = 0;
    int End = 0;
};
int SCPSection4::SCPQRSProtected::Size = 8;

// Defined in SCP.
ushort SCPSection4::_SectionID = 4;

/// <summary>
/// Class contains section 4 (QRS locations).
/// </summary>
SCPSection4::SCPSection4()
{
    // Part of the stored Data Structure.
    _MedianDataLength = 0;
    _FirstFiducial = 0;
    _NrQRS = 0xffff;
    _Subtraction.clear();
    _Protected.clear();
}
int SCPSection4::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_MedianDataLength, buffer, bufferLength, offset, sizeof(_MedianDataLength), true);
    offset += sizeof(_MedianDataLength);
    BytesTool::writeBytes(_FirstFiducial, buffer, bufferLength, offset, sizeof(_FirstFiducial), true);
    offset += sizeof(_FirstFiducial);
    BytesTool::writeBytes(_NrQRS, buffer, bufferLength, offset, sizeof(_NrQRS), true);
    offset += sizeof(_NrQRS);

    for (int loper = 0; loper < _NrQRS; loper++) {
        int err = _Subtraction[loper].Write(buffer, bufferLength, offset);

        if (err != 0) {
            return err << loper;
        }

        offset += SCPQRSSubtraction::Size;
    }

    if (_Protected.size() > 0) {
        for (int loper = 0; loper < _NrQRS; loper++) {
            int err = _Protected[loper].Write(buffer, bufferLength, offset);

            if (err != 0) {
                return err << loper;
            }

            offset += SCPQRSProtected::Size;
        }
    }

    return 0x0;
}
void SCPSection4::_Empty()
{
    _MedianDataLength = 0;
    _FirstFiducial = 0;
    _NrQRS = 0xffff;
    _Subtraction.clear();
    _Protected.clear();
}
int SCPSection4::_getLength()
{
    if (Works()) {
        int sum = (sizeof(_MedianDataLength) + sizeof(_FirstFiducial) + sizeof(_NrQRS));
        sum += (_NrQRS * SCPQRSSubtraction::Size);

        if (_Protected.size() > 0) {
            sum += (_NrQRS * SCPQRSProtected::Size);
        }

        return ((sum % 2) == 0 ? sum : sum + 1);
    }

    return 0;
}
ushort SCPSection4::getSectionID()
{
    return _SectionID;
}
bool SCPSection4::Works()
{
    if ((_Subtraction.size() > 0)
        && (_Protected.size() > 0)
        && (_NrQRS == _Subtraction.size())
        && (_NrQRS == _Protected.size())
        || (_NrQRS == 0)) {
        if ((_Protected.size() > 0)
            && (_Protected.size() != _NrQRS)) {
            return false;
        }

#if 0

        for (int loper = 0; loper < _NrQRS; loper++) {
            if ((_Subtraction[loper] == null)
                || ((_Protected.size() > 0)
                    && (_Protected[loper] == null))) {
                return false;
            }
        }

#endif
        return true;
    }

    return false;
}
#if 0
/// <summary>
/// Function to add median data to residual data.
/// </summary>
/// <remarks>both signals must have the same sample rate and AVM</remarks>
/// <param name="definition">Data structure containing information about length of residual data.</param>
/// <param name="residual">2D array containing residual data for each lead. On succes will contain rhythm data.</param>
/// <param name="median">2D array containing median data for each lead.</param>
/// <returns>error:
/// 0x001) given data makes no sense.
/// 0x002) Fault in Lead nr 0.
/// 0x004) Fault in Lead nr 1.
/// 0x008) Fault in Lead nr 2.
/// 0x010) Fault in Lead nr 3.
/// 0x020) Fault in Lead nr 4.
/// 0x040) Fault in Lead nr 5.
/// 0x080) Fault in Lead nr 6.
/// 0x100) Fault in Lead nr 7.
/// ...</returns>
int SCPSection4::AddMedians(SCPSection3 definition, short[][] residual, short[][] median)
{
    // Check if given data makes sense
    if (Works()
        && (definition != null)
        && (residual != null)
        && (median != null)
        && (definition.Works())
        && (median.Length == definition.getNrLeads())
        && (residual.Length == median.Length)) {
        int err = 0;

        for (int qrsnr = 0; qrsnr < _NrQRS; qrsnr++) {
            if ((_Subtraction[qrsnr].Type != 0)) {
                continue;
            }

            for (int channel = 0; channel < median.Length; channel++) {
                if ((residual[channel] == null)
                    || (median[channel] == null)
                    || (residual[channel].Length < definition.getLeadLength(channel))) {
                    err |= (0x2 << channel);
                    continue;
                }

                int loperResidual = _Subtraction[qrsnr].Start - definition.getLeadStart(channel);
                int loperMedian = (_FirstFiducial - _Subtraction[qrsnr].Fiducial + _Subtraction[qrsnr].Start - 1);
                int endResidual = _Subtraction[qrsnr].End - definition.getLeadStart(channel);

                if ((loperResidual >= 0)
                    && (loperMedian >= 0)) {
                    while ((loperResidual <= endResidual)
                           && (loperMedian < median[channel].Length)) {
                        residual[channel][loperResidual++] += median[channel][loperMedian++];
                    }
                }
            }
        }

        return err;
    }

    return -1;
}
/// <summary>
/// Function to subtract median data to residual data.
/// </summary>
/// <remarks>both signals must have the same sample rate and AVM</remarks>
/// <param name="definition">Data structure containing information about length of rhythm data.</param>
/// <param name="rhythm">2D array containing rhythm data for each lead. On succes will contain residual data.</param>
/// <param name="median">2D array containing median data for each lead.</param>
/// <returns>error:
/// 0x001) given data makes no sense.
/// 0x002) Fault in Lead nr 0.
/// 0x004) Fault in Lead nr 1.
/// 0x008) Fault in Lead nr 2.
/// 0x010) Fault in Lead nr 3.
/// 0x020) Fault in Lead nr 4.
/// 0x040) Fault in Lead nr 5.
/// 0x080) Fault in Lead nr 6.
/// 0x100) Fault in Lead nr 7.
/// ...</returns>
int SCPSection4::SubtractMedians(SCPSection3 definition, short[][] rhythm, short[][] median)
{
    // Check if given data makes sense
    if (Works()
        && (definition != null)
        & (rhythm != null)
        && (median != null)
        && (definition.Works())
        && (median.Length == definition.getNrLeads())
        && (rhythm.Length == median.Length)) {
        int err = 0;

        for (int qrsnr = 0; qrsnr < _NrQRS; qrsnr++) {
            if ((_Subtraction[qrsnr].Type != 0)) {
                continue;
            }

            for (int channel = 0; channel < median.Length; channel++) {
                if ((rhythm[channel] == null)
                    || (median[channel] == null)
                    || (rhythm[channel].Length < definition.getLeadLength(channel))) {
                    err |= (0x2 << channel);
                    continue;
                }

                int loperResidual = _Subtraction[qrsnr].Start - definition.getLeadStart(channel);
                int loperMedian = (_FirstFiducial - _Subtraction[qrsnr].Fiducial + _Subtraction[qrsnr].Start - 1);
                int endResidual = _Subtraction[qrsnr].End - definition.getLeadStart(channel);

                if ((loperResidual >= 0)
                    && (loperMedian >= 0)) {
                    while ((loperResidual <= endResidual)
                           && (loperMedian < median[channel].Length)) {
                        rhythm[channel][loperResidual++] -= median[channel][loperMedian++];
                    }
                }
            }
        }

        return err;
    }

    return -1;
}
#endif
/// <summary>
/// Function to set protected zones using global measurements.
/// </summary>
/// <param name="global">global measurments</param>
/// <param name="medianFreq">Samples per Second of median</param>
/// <param name="rate">Bimodal compression rate</param>
/// <param name="minbegin">Begin of all leads in samples</param>
/// <param name="maxend">End of all leads in samples</param>
void SCPSection4::setProtected(GlobalMeasurements& global, int medianFreq, int rate, int minbegin, int maxend)
{
    if ((_Subtraction.size() > 0)
        && (_Protected.size() > 0)
        && (medianFreq != 0)) {
        // If global measurements are per beat use them.
        if (global.measurment.size() == (_Protected.size() + 1)) {
            for (int loper = 0; loper < _Protected.size(); loper++) {
                _Protected[loper].Start = _Subtraction[loper].Fiducial + (short)((global.measurment[loper + 1].QRSonset - (_FirstFiducial * 1000)) / medianFreq);
                _Protected[loper].End = _Subtraction[loper].Fiducial + (short)((global.measurment[loper + 1].QRSoffset - (_FirstFiducial * 1000)) / medianFreq);
                // Make protected zones work properly
                _Protected[loper].Start -= ((_Protected[loper].Start % rate) == 0 ? rate : (_Protected[loper].Start % rate));
                _Protected[loper].Start++;
                _Protected[loper].End += (rate - (_Protected[loper].End % rate));

                // Keep it all between boundaries of ECG.
                if (_Protected[loper].Start < minbegin) {
                    _Protected[loper].Start = minbegin;
                }

                if (_Protected[loper].End > maxend) {
                    _Protected[loper].End = maxend;
                }
            }
        }
        else if (global.measurment.size() > 0) {
            for (int loper = 0; loper < _Protected.size(); loper++) {
                if (_Subtraction[loper].Type == 0) {
                    _Protected[loper].Start = _Subtraction[loper].Fiducial + (short)((global.measurment[0].QRSonset * medianFreq) / 1000) - _FirstFiducial;
                    _Protected[loper].End = _Subtraction[loper].Fiducial + (short)((global.measurment[0].QRSoffset * medianFreq) / 1000) - _FirstFiducial;
                }

                // Make protected zones work properly
                _Protected[loper].Start -= ((_Protected[loper].Start % rate) == 0 ? rate : (_Protected[loper].Start % rate));
                _Protected[loper].Start++;
                _Protected[loper].End += (rate - (_Protected[loper].End % rate));

                // Keep it all between boundaries of ECG.
                if (_Protected[loper].Start < minbegin) {
                    _Protected[loper].Start = minbegin;
                }

                if (_Protected[loper].End > maxend) {
                    _Protected[loper].End = maxend;
                }
            }
        }
    }
}
int SCPSection4::setSignals(Signals& signals)
{
    if (signals.getNrLeads() != 0) {
        _MedianDataLength = signals.MedianLength;
        _FirstFiducial = signals.MedianFiducialPoint;

        if (signals.qrsZone.size() == 0) {
            _NrQRS = 0;
            _Subtraction.clear();
            _Protected.clear();
        }
        else {
            _NrQRS = (ushort) signals.qrsZone.size();
            _Subtraction.resize(_NrQRS);
            _Protected.resize(_NrQRS);

            for (int loper = 0; loper < _NrQRS; loper++) {
                _Subtraction[loper].Type = signals.qrsZone[loper].Type;
                _Subtraction[loper].Fiducial = signals.qrsZone[loper].Fiducial;

                if (_Subtraction[loper].Type == 0) {
                    _Subtraction[loper].Start = signals.qrsZone[loper].Start + 1;
                    _Subtraction[loper].End = signals.qrsZone[loper].End;

                    if (((_Subtraction[loper].End - _Subtraction[loper].Fiducial) + _FirstFiducial)
                        >= ((signals.MedianLength * signals.MedianSamplesPerSecond) / 1000)) {
                        _Subtraction[loper].End = (int)(((((signals.MedianLength * signals.MedianSamplesPerSecond) / 1000) - _FirstFiducial) + _Subtraction[loper].Fiducial - 2) & 0xfffffffe);
                    }

                    _Protected[loper].Start = _Subtraction[loper].Start;
                    _Protected[loper].End = _Subtraction[loper].End;
                }
            }
        }

        return 0;
    }

    return 1;
}
}
}
