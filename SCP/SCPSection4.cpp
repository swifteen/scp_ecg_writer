#include "SCPSection4.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class containing SCP QRS subtraction zone.
/// </summary>
class SCPSection4::SCPQRSSubtraction
{
    /// <summary>
    /// Constructor for an QRS Subtraction zone.
    /// </summary>
    SCPQRSSubtraction():Size(14)
    {
        ushort Type = ushort.MaxValue;
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
    SCPQRSSubtraction(byte type, int start, int fiducial, int end)
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
    int Write(byte[] buffer, int offset)
    {
        if (offset + Size > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Type, buffer, offset, sizeof(Type), true);
        offset += sizeof(Type);
        BytesTool::writeBytes(Start, buffer, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(Fiducial, buffer, offset, sizeof(Fiducial), true);
        offset += sizeof(Fiducial);
        BytesTool::writeBytes(End, buffer, offset, sizeof(End), true);
        offset += sizeof(End);

        return 0x0;
    }
public:
    const int Size;
    ushort Type;
    int Start;
    int Fiducial;
    int End;
};

/// <summary>
/// Class containing QRS protected zones.
/// </summary>
class SCPSection4::SCPQRSProtected
{
    /// <summary>
    /// Constructor to create an QRS protected zone.
    /// </summary>
    SCPQRSProtected():Size(8)
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
    int Write(byte[] buffer, int offset)
    {
        if (offset + Size > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Start, buffer, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(End, buffer, offset, sizeof(End), true);
        offset += sizeof(End);

        return 0x0;
    }
public:
    const int Size;

    int Start = 0;
    int End = 0;
};

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
    _Subtraction = null;
    _Protected = null;
}
int SCPSection4::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_MedianDataLength, buffer, offset, sizeof(_MedianDataLength), true);
    offset += sizeof(_MedianDataLength);
    BytesTool::writeBytes(_FirstFiducial, buffer, offset, sizeof(_FirstFiducial), true);
    offset += sizeof(_FirstFiducial);
    BytesTool::writeBytes(_NrQRS, buffer, offset, sizeof(_NrQRS), true);
    offset += sizeof(_NrQRS);
    for (int loper=0;loper < _NrQRS;loper++)
    {
        int err = _Subtraction[loper].Write(buffer, offset);
        if (err != 0)
        {
            return err << loper;
        }
        offset += SCPQRSSubtraction.Size;
    }

    if (_Protected != null)
    {
        for (int loper=0;loper < _NrQRS;loper++)
        {
            int err = _Protected[loper].Write(buffer, offset);
            if (err != 0)
            {
                return err << loper;
            }
            offset += SCPQRSProtected.Size;
        }
    }
    return 0x0;
}
void SCPSection4::_Empty()
{
    _MedianDataLength = 0;
    _FirstFiducial = 0;
    _NrQRS = 0xffff;
    _Subtraction = null;
    _Protected = null;
}
int SCPSection4::_getLength()
{
    if (Works())
    {
        int sum = (sizeof(_MedianDataLength) + sizeof(_FirstFiducial) + sizeof(_NrQRS));
        sum += (_NrQRS * SCPQRSSubtraction.Size);
        if (_Protected != null)
        {
            sum += (_NrQRS * SCPQRSProtected.Size);
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
    if ((_Subtraction != null)
            &&  (_Protected != null)
            &&  (_NrQRS == _Subtraction.Length)
            &&  (_NrQRS == _Protected.Length)
            ||  (_NrQRS == 0))
    {
        if ((_Protected != null)
                &&  (_Protected.Length != _NrQRS))
        {
            return false;
        }

        for (int loper=0;loper < _NrQRS;loper++)
        {
            if ((_Subtraction[loper] == null)
                    ||  ((_Protected != null)
                         &&	 (_Protected[loper] == null)))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
/// <summary>
/// Function to get length of median.
/// </summary>
/// <returns>length of median data.</returns>
ushort SCPSection4::getMedianLength()
{
    if (Works())
    {
        return _MedianDataLength;
    }
    return 0;
}
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
            &&  (definition != null)
            &&	(residual != null)
            &&  (median != null)
            &&  (definition.Works())
            &&  (median.Length == definition.getNrLeads())
            &&  (residual.Length == median.Length))
    {
        int err = 0;
        for (int qrsnr=0;qrsnr < _NrQRS;qrsnr++)
        {
            if ((_Subtraction[qrsnr].Type != 0))
                continue;

            for (int channel=0;channel < median.Length;channel++)
            {
                if ((residual[channel] == null)
                        ||  (median[channel] == null)
                        ||  (residual[channel].Length < definition.getLeadLength(channel)))
                {
                    err |= (0x2 << channel);
                    continue;
                }

                int loperResidual = _Subtraction[qrsnr].Start - definition.getLeadStart(channel);
                int loperMedian = (_FirstFiducial - _Subtraction[qrsnr].Fiducial + _Subtraction[qrsnr].Start - 1);
                int endResidual = _Subtraction[qrsnr].End - definition.getLeadStart(channel);

                if ((loperResidual >= 0)
                        &&  (loperMedian >= 0))
                {
                    while ((loperResidual <= endResidual)
                           && (loperMedian < median[channel].Length))
                    {
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
            &&  (definition != null)
            &	(rhythm != null)
            &&  (median != null)
            &&  (definition.Works())
            &&  (median.Length == definition.getNrLeads())
            &&  (rhythm.Length == median.Length))
    {
        int err = 0;
        for (int qrsnr=0;qrsnr < _NrQRS;qrsnr++)
        {
            if ((_Subtraction[qrsnr].Type != 0))
                continue;

            for (int channel=0;channel < median.Length;channel++)
            {
                if ((rhythm[channel] == null)
                        ||  (median[channel] == null)
                        ||  (rhythm[channel].Length < definition.getLeadLength(channel)))
                {
                    err |= (0x2 << channel);
                    continue;
                }

                int loperResidual = _Subtraction[qrsnr].Start - definition.getLeadStart(channel);
                int loperMedian = (_FirstFiducial - _Subtraction[qrsnr].Fiducial + _Subtraction[qrsnr].Start - 1);
                int endResidual = _Subtraction[qrsnr].End - definition.getLeadStart(channel);

                if ((loperResidual >= 0)
                        &&  (loperMedian >= 0))
                {
                    while ((loperResidual <= endResidual)
                           && (loperMedian < median[channel].Length))
                    {
                        rhythm[channel][loperResidual++] -= median[channel][loperMedian++];
                    }
                }
            }
        }
        return err;
    }
    return -1;
}
/// <summary>
/// Function to get nr of protected zones.
/// </summary>
/// <returns>nr of protected zones</returns>
int SCPSection4::getNrProtectedZones()
{
    return (_Protected != null ? _Protected.Length : 0);
}
/// <summary>
/// Function to get start of protected zone.
/// </summary>
/// <param name="nr">nr of protected zone</param>
/// <returns>start sample nr of protected zone</returns>
int SCPSection4::getProtectedStart(int nr)
{
    if ((_Protected != null)
            &&  (nr >= 0)
            &&	(nr < _Protected.Length))
    {
        return _Protected[nr].Start;
    }
    return -1;
}
/// <summary>
/// Function to get end of protected zone.
/// </summary>
/// <param name="nr">nr of protected zone</param>
/// <returns>end sample nr of protected zone</returns>
int SCPSection4::getProtectedEnd(int nr)
{
    if ((_Protected != null)
            &&  (nr >= 0)
            &&	(nr < _Protected.Length))
    {
        return _Protected[nr].End;
    }
    return -1;
}
/// <summary>
/// Function to get length of protected zone.
/// </summary>
/// <param name="nr">nr of protected zone</param>
/// <returns>length of protected zone in samples</returns>
int SCPSection4::getProtectedLength(int nr)
{
    if ((_Protected != null)
            &&  (nr >= 0)
            &&	(nr < _Protected.Length))
    {
        if (_Protected[nr] == null)
        {
            return 0;
        }

        int templen = _Protected[nr].End - _Protected[nr].Start;
        return (templen > 0 ? templen + 1 : 0);
    }
    return -1;
}
/// <summary>
/// Function to set protected zones using global measurements.
/// </summary>
/// <param name="global">global measurments</param>
/// <param name="medianFreq">Samples per Second of median</param>
/// <param name="rate">Bimodal compression rate</param>
/// <param name="minbegin">Begin of all leads in samples</param>
/// <param name="maxend">End of all leads in samples</param>
void SCPSection4::setProtected(GlobalMeasurements global, int medianFreq, int rate, int minbegin, int maxend)
{
    if ((global != null)
            &&	(global.measurment != null)
            &&	(_Subtraction != null)
            &&	(_Protected != null)
            &&	(medianFreq != 0))
    {
        // If global measurements are per beat use them.
        if (global.measurment.Length == (_Protected.Length + 1))
        {
            for (int loper=0;loper < _Protected.Length;loper++)
            {
                _Protected[loper].Start = _Subtraction[loper].Fiducial + (short) ((global.measurment[loper + 1].QRSonset - (_FirstFiducial * 1000)) / medianFreq);
                _Protected[loper].End = _Subtraction[loper].Fiducial + (short) ((global.measurment[loper + 1].QRSoffset - (_FirstFiducial * 1000)) / medianFreq);

                // Make protected zones work properly
                _Protected[loper].Start -= ((_Protected[loper].Start % rate) == 0 ? rate : (_Protected[loper].Start % rate));
                _Protected[loper].Start++;
                _Protected[loper].End += (rate - (_Protected[loper].End % rate));

                // Keep it all between boundaries of ECG.
                if (_Protected[loper].Start < minbegin)
                {
                    _Protected[loper].Start = minbegin;
                }
                if (_Protected[loper].End > maxend)
                {
                    _Protected[loper].End = maxend;
                }
            }
        }
        else if (global.measurment.Length > 0)
        {
            for (int loper=0;loper < _Protected.Length;loper++)
            {
                if (_Subtraction[loper].Type == 0)
                {
                    _Protected[loper].Start = _Subtraction[loper].Fiducial + (short) ((global.measurment[0].QRSonset * medianFreq) / 1000) - _FirstFiducial;
                    _Protected[loper].End = _Subtraction[loper].Fiducial + (short) ((global.measurment[0].QRSoffset * medianFreq) / 1000) - _FirstFiducial;
                }

                // Make protected zones work properly
                _Protected[loper].Start -= ((_Protected[loper].Start % rate) == 0 ? rate : (_Protected[loper].Start % rate));
                _Protected[loper].Start++;
                _Protected[loper].End += (rate - (_Protected[loper].End % rate));

                // Keep it all between boundaries of ECG.
                if (_Protected[loper].Start < minbegin)
                {
                    _Protected[loper].Start = minbegin;
                }
                if (_Protected[loper].End > maxend)
                {
                    _Protected[loper].End = maxend;
                }
            }
        }
    }
}
// Signal Manupalations
int SCPSection4::getSignals(out Signals signals)
{
    signals = new Signals();
    int err = getSignalsToObj(signals);
    if (err != 0)
    {
        signals = null;
    }
    return err;
}
int SCPSection4::getSignalsToObj(Signals signals)
{
    if (signals != null
            &&  Works())
    {
        signals.MedianLength = _MedianDataLength;
        signals.MedianFiducialPoint = _FirstFiducial;

        if (_NrQRS == 0)
        {
            signals.QRSZone = null;
        }
        else
        {
            signals.QRSZone = new QRSZone[_NrQRS];
            for (int loper = 0;loper < _NrQRS;loper++)
            {
                signals.QRSZone[loper] = new QRSZone();
                if (_Subtraction[loper] != null)
                {
                    signals.QRSZone[loper].Type = _Subtraction[loper].Type;
                    signals.QRSZone[loper].Start = _Subtraction[loper].Start - 1;
                    signals.QRSZone[loper].Fiducial = _Subtraction[loper].Fiducial;
                    signals.QRSZone[loper].End = _Subtraction[loper].End;
                }
            }
        }

        return 0;
    }
    return 1;
}
int SCPSection4::setSignals(Signals signals)
{
    if ((signals != null)
            &&  (signals.NrLeads != 0))
    {
        _MedianDataLength = signals.MedianLength;
        _FirstFiducial = signals.MedianFiducialPoint;

        if (signals.QRSZone == null)
        {
            _NrQRS = 0;
            _Subtraction = null;
            _Protected = null;
        }
        else
        {
            _NrQRS = (ushort) signals.QRSZone.Length;
            _Subtraction = new SCPQRSSubtraction[_NrQRS];
            _Protected = new SCPQRSProtected[_NrQRS];

            for (int loper=0;loper < _NrQRS;loper++)
            {
                _Subtraction[loper] = new SCPQRSSubtraction();
                _Protected[loper] = new SCPQRSProtected();

                if (signals.QRSZone[loper] != null)
                {
                    _Subtraction[loper].Type = signals.QRSZone[loper].Type;
                    _Subtraction[loper].Fiducial = signals.QRSZone[loper].Fiducial;

                    if (_Subtraction[loper].Type == 0)
                    {
                        _Subtraction[loper].Start = signals.QRSZone[loper].Start + 1;
                        _Subtraction[loper].End = signals.QRSZone[loper].End;

                        if (((_Subtraction[loper].End - _Subtraction[loper].Fiducial) + _FirstFiducial)
                                >=	((signals.MedianLength * signals.MedianSamplesPerSecond) / 1000))
                        {
                            _Subtraction[loper].End = (int) (((((signals.MedianLength * signals.MedianSamplesPerSecond) / 1000) - _FirstFiducial) + _Subtraction[loper].Fiducial - 2) & 0xfffffffe);
                        }

                        _Protected[loper].Start = _Subtraction[loper].Start;
                        _Protected[loper].End = _Subtraction[loper].End;
                    }
                    else
                    {
                    }
                }
            }
        }

        return 0;
    }
    return 1;
}
}
}
