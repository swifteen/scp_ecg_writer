#include "SCPSection7.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGGlobalMeasurements;

namespace ECGConversion
{
namespace SCP
{

class SCPSection7::SCPExtraMeasurements
{
public:
    SCPExtraMeasurements()
    {
        VentRate = GlobalMeasurement::NoValue;
        AtrialRate = GlobalMeasurement::NoValue;
        QTc = GlobalMeasurement::NoValue;
        FormulaType = 0;
        TaggedFieldSize = 0;
#if 0
        // Tagged Field are not implemented.
        TaggedFields = null;
#endif
    }
    /// <summary>
    /// Function to read SCP extra measurements.
    /// </summary>
    /// <param name="buffer">byte array to read from</param>
    /// <param name="offset">position to start reading</param>
    /// <returns>0 on success</returns>

    /// <summary>
    /// Function to write SCP extra measurements.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        BytesTool::writeBytes(VentRate, buffer, bufferLength, offset, sizeof(VentRate), true);
        offset += sizeof(VentRate);
        BytesTool::writeBytes(AtrialRate, buffer, bufferLength, offset, sizeof(AtrialRate), true);
        offset += sizeof(AtrialRate);
        BytesTool::writeBytes(QTc, buffer, bufferLength, offset, sizeof(QTc), true);
        offset += sizeof(QTc);
        BytesTool::writeBytes(FormulaType, buffer, bufferLength, offset, sizeof(FormulaType), true);
        offset += sizeof(FormulaType);
        BytesTool::writeBytes(TaggedFieldSize, buffer, bufferLength, offset, sizeof(TaggedFieldSize), true);
        offset += sizeof(TaggedFieldSize);
#if 0

        if (TaggedFields != null) {
            offset += BytesTool::copy(buffer, offset, TaggedFields, 0, TaggedFieldSize);
        }

#endif
        return 0x0;
    }
    /// <summary>
    /// Function to empty this extra measurements.
    /// </summary>
    void Empty()
    {
        VentRate = 0;
        AtrialRate = 0;
        QTc = 0;
        FormulaType = 0;
        TaggedFieldSize = 0;
#if 0
        TaggedFields = null;
#endif
    }
    /// <summary>
    /// Function to get length of extra measurements.
    /// </summary>
    /// <returns>length of extra measurements</returns>
    int getLength()
    {
        if (Works()) {
            int sum = sizeof(VentRate) + sizeof(AtrialRate);
            sum += sizeof(QTc) + sizeof(FormulaType) + sizeof(TaggedFieldSize);
            sum += TaggedFieldSize;
            return ((sum % 2) == 0 ? sum : sum + 1);
        }

        return 0;
    }
    /// <summary>
    /// Function to check if extra measurements works
    /// </summary>
    /// <returns>if writeable is true</returns>
    bool Works()
    {
        if (TaggedFieldSize == 0) {
            return true;
        }

#if 0
        else if ((TaggedFields != null)
                 && (TaggedFields.Length >= TaggedFieldSize)) {
            return true;
        }

#endif
        return false;
    }
public:
    ushort VentRate ;
    ushort AtrialRate;
    ushort QTc;
    uchar FormulaType;
    ushort TaggedFieldSize;
#if 0
    // Tagged Field are not implemented.
    uchar* TaggedFields;
#endif
};


/// <summary>
/// Class containing SCP Spike info.
/// </summary>
class SCPSection7::SCPSpikeInfo
{
public:
    SCPSpikeInfo()
    {
        Type = 255;
        Source = 0;
        TriggerIndex = GlobalMeasurement::NoValue;
        ushort PulseWidth = GlobalMeasurement::NoValue;
    }

    /// <summary>
    /// Function to write SCP Spike info.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if ((offset + Size) > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Type, buffer, bufferLength, offset, sizeof(Type), true);
        offset += sizeof(Type);
        BytesTool::writeBytes(Source, buffer, bufferLength, offset, sizeof(Source), true);
        offset += sizeof(Source);
        BytesTool::writeBytes(TriggerIndex, buffer, bufferLength, offset, sizeof(TriggerIndex), true);
        offset += sizeof(TriggerIndex);
        BytesTool::writeBytes(PulseWidth, buffer, bufferLength, offset, sizeof(PulseWidth), true);
        offset += sizeof(PulseWidth);
        return 0x0;
    }
public:
    static int Size ;
    uchar Type;
    uchar Source;
    ushort TriggerIndex ;
    ushort PulseWidth;
};
int SCPSection7::SCPSpikeInfo::Size = 9;

/// <summary>
/// Class containing measurements for SCP.
/// </summary>
class SCPSection7::SCPMeasurement
{
public:
    SCPMeasurement()
    {
        Ponset = GlobalMeasurement::NoValue;
        Poffset = GlobalMeasurement::NoValue;
        QRSonset = GlobalMeasurement::NoValue;
        QRSoffset = GlobalMeasurement::NoValue;
        Toffset = GlobalMeasurement::NoValue;
        Paxis = GlobalMeasurement::NoAxisValue;
        QRSaxis = GlobalMeasurement::NoAxisValue;
        Taxis = GlobalMeasurement::NoAxisValue;
    }

    /// <summary>
    /// Function to write measurements.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if ((offset + Size) > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Ponset, buffer, bufferLength, offset, sizeof(Ponset), true);
        offset += sizeof(Ponset);
        BytesTool::writeBytes(Poffset, buffer, bufferLength, offset, sizeof(Poffset), true);
        offset += sizeof(Poffset);
        BytesTool::writeBytes(QRSonset, buffer, bufferLength, offset, sizeof(QRSonset), true);
        offset += sizeof(QRSonset);
        BytesTool::writeBytes(QRSoffset, buffer, bufferLength, offset, sizeof(QRSoffset), true);
        offset += sizeof(QRSoffset);
        BytesTool::writeBytes(Toffset, buffer, bufferLength, offset, sizeof(Toffset), true);
        offset += sizeof(Toffset);
        BytesTool::writeBytes(Paxis, buffer, bufferLength, offset, sizeof(Paxis), true);
        offset += sizeof(Paxis);
        BytesTool::writeBytes(QRSaxis, buffer, bufferLength, offset, sizeof(QRSaxis), true);
        offset += sizeof(QRSaxis);
        BytesTool::writeBytes(Taxis, buffer, bufferLength, offset, sizeof(Taxis), true);
        offset += sizeof(Taxis);
        return 0x0;
    }
public:
    static int Size ;
    ushort Ponset ;
    ushort Poffset ;
    ushort QRSonset ;
    ushort QRSoffset ;
    ushort Toffset ;
    short Paxis ;
    short QRSaxis;
    short Taxis ;
};
int SCPSection7::SCPMeasurement::Size = 16;

/// <summary>
/// Class containing SCP spikes.
/// </summary>
class SCPSection7::SCPSpike
{
public:
    SCPSpike()
    {
        Time = GlobalMeasurement::NoValue;
        Amplitude = GlobalMeasurement::NoAxisValue;
    }
    /// <summary>
    /// Function to write SCP spike.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if ((offset + Size) > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Time, buffer, bufferLength, offset, sizeof(Time), true);
        offset += sizeof(Time);
        BytesTool::writeBytes(Amplitude, buffer, bufferLength, offset, sizeof(Amplitude), true);
        offset += sizeof(Amplitude);
        return 0x0;
    }
public:
    static int Size;
    ushort Time;
    short Amplitude;
};
int SCPSection7::SCPSpike::Size = 4;
/// <summary>
/// Class contains section 7 (contains the global measurements section).
/// </summary>

ushort SCPSection7::_SectionID = 7;

SCPSection7::SCPSection7()
{
    // Special variables for this section.
    _AfterSpikes = false;
    _AfterSpikesInfo = false;
    _AfterQRSType = false;
    // Part of the stored Data Structure.
    _NrRefTypeQRS = 0;
    _NrSpikes = 0;
    _AvgRRInterval = 0;
    _AvgPPInterval = 0;
    _Measurements.clear();
    _Spikes.clear();
    _SpikesInfo.clear();
    _NrQRS = 0;
    _QRSType = null;
    _ExtraMeasurements = new SCPExtraMeasurements;
    // Manufactor specific block (Not implemented, because UNIPRO doesn't store this kind of info).
    _Rest = null;
}

SCPSection7::~SCPSection7()
{
    delete[] _QRSType;
    delete[] _Rest;
    delete _ExtraMeasurements;
}

int SCPSection7::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_NrRefTypeQRS, buffer, bufferLength, offset, sizeof(_NrRefTypeQRS), true);
    offset += sizeof(_NrRefTypeQRS);
    BytesTool::writeBytes(_NrSpikes, buffer, bufferLength, offset, sizeof(_NrSpikes), true);
    offset += sizeof(_NrSpikes);
    BytesTool::writeBytes(_AvgRRInterval, buffer, bufferLength, offset, sizeof(_AvgRRInterval), true);
    offset += sizeof(_AvgRRInterval);
    BytesTool::writeBytes(_AvgPPInterval, buffer, bufferLength, offset, sizeof(_AvgPPInterval), true);
    offset += sizeof(_AvgPPInterval);

    if (_NrRefTypeQRS > 0) {
        for (int loper = 0; loper < _NrRefTypeQRS; loper++) {
            int err = _Measurements[loper].Write(buffer, bufferLength, offset);

            if (err != 0) {
                return 0x1;
            }

            offset += SCPMeasurement::Size;
        }
    }

    if (_NrSpikes > 0) {
        for (int loper = 0; loper < _NrSpikes; loper++) {
            int err = _Spikes[loper].Write(buffer, bufferLength, offset);

            if (err != 0) {
                return 0x2;
            }

            offset += SCPSpike::Size;
        }

        if (!_AfterSpikes) {
            return 0;
        }

        for (int loper = 0; loper < _NrSpikes; loper++) {
            int err = _SpikesInfo[loper].Write(buffer, bufferLength, offset);

            if (err != 0) {
                return 0x4;
            }

            offset += SCPSpikeInfo::Size;
        }
    }

    if (!_AfterSpikesInfo) {
        return 0;
    }

    BytesTool::writeBytes(_NrQRS, buffer, bufferLength, offset, sizeof(_NrQRS), true);
    offset += sizeof(_NrQRS);

    if ((_NrQRS > 0)
        && (_QRSType != null)) {
        for (int loper = 0; loper < _NrQRS; loper++) {
            BytesTool::writeBytes(_QRSType[loper], buffer, bufferLength, offset, sizeof(_QRSType[loper]), true);
            offset += sizeof(_QRSType[loper]);
        }
    }

    if (!_AfterQRSType) {
        return 0;
    }

    if (_ExtraMeasurements != null) {
        int err = _ExtraMeasurements->Write(buffer, bufferLength, offset);

        if (err != 0) {
            return 0x8;
        }

        offset += _ExtraMeasurements->getLength();
    }

#if 0 //for read

    if ((_Rest != null)
        && ((offset + _Rest.Length) < buffer.Length)) {
        offset += BytesTool::copy(_Rest, 0, buffer, bufferLength, offset, _Rest.Length);
    }

#endif
    return 0x0;
}

void SCPSection7::_Empty()
{
    _NrRefTypeQRS = 0;
    _NrSpikes = 0;
    _AvgRRInterval = 0;
    _AvgPPInterval = 0;
    _Measurements.clear();
    _Spikes.clear();
    _SpikesInfo.clear();
    _AfterSpikes = false;
    _NrQRS = 0;
    _AfterQRSType = false;
    delete[] _QRSType;
    _QRSType = null;
    delete[] _Rest;
    _Rest = null;
    _ExtraMeasurements->Empty();
}

int SCPSection7::_getLength()
{
    if (Works()) {
        int sum = sizeof(_NrRefTypeQRS) + sizeof(_NrSpikes) + sizeof(_AvgPPInterval) + sizeof(_AvgRRInterval);
        sum += _NrRefTypeQRS * SCPMeasurement::Size;
        sum += (_NrSpikes * (SCPSpike::Size + SCPSpikeInfo::Size));

        if (_AfterSpikes) {
            sum += sizeof(_NrQRS) + (_NrQRS * sizeof(uchar));

            if (_AfterQRSType) {
                sum += _ExtraMeasurements->getLength();
#if 0 //for read

                if (_Rest != null) {
                    sum += _Rest.Length;
                }

#endif
            }
        }

        return sum;
    }

    return 0;
}

ushort SCPSection7::getSectionID()
{
    return _SectionID;
}

bool SCPSection7::Works()
{
    if ((_NrRefTypeQRS == 0)
        || (_Measurements.size() == _NrRefTypeQRS)) {
        if ((_NrSpikes == 0)
            || (_Spikes.size() == _NrSpikes)) {
            if (!_AfterSpikes) {
                return ((_NrRefTypeQRS != 0) || (_NrSpikes != 0));
            }

            if (_SpikesInfo.size() == _NrSpikes) {
                if (!_AfterSpikesInfo) {
                    return ((_NrRefTypeQRS != 0) || (_NrSpikes != 0));
                }

                if ((_NrQRS == 0)
                    || (_QRSType != null)) {
                    return (!_AfterQRSType
                            || (_ExtraMeasurements != null));
                }
            }
        }

        return true;
    }

    return false;
}

int SCPSection7::setGlobalMeasurements(GlobalMeasurements& mes)
{
    if (mes.measurment.size() > 0) {
        Empty();
        _AvgRRInterval = mes.AvgRR;
        _AvgPPInterval = mes.AvgPP;
        _NrRefTypeQRS = (uchar) mes.measurment.size();
        _Measurements.resize(_NrRefTypeQRS);

        for (int loper = 0; loper < _NrRefTypeQRS; loper++) {
            _Measurements[loper].Ponset = mes.measurment[loper].Ponset;
            _Measurements[loper].Poffset = mes.measurment[loper].Poffset;
            _Measurements[loper].QRSonset = mes.measurment[loper].QRSonset;
            _Measurements[loper].QRSoffset = mes.measurment[loper].QRSoffset;
            _Measurements[loper].Toffset = mes.measurment[loper].Toffset;
            _Measurements[loper].Paxis = mes.measurment[loper].Paxis;
            _Measurements[loper].QRSaxis = mes.measurment[loper].QRSaxis;
            _Measurements[loper].Taxis = mes.measurment[loper].Taxis;
        }

        _NrSpikes = 0;

        if (mes.spike.size() > 0) {
            _NrSpikes = (uchar) mes.spike.size();
            _Spikes.resize(_NrSpikes);
            _SpikesInfo.resize(_NrSpikes);

            for (int loper = 0; loper < _NrSpikes; loper++) {
                _Spikes[loper].Time = mes.spike[loper].Time;
                _Spikes[loper].Amplitude = mes.spike[loper].Amplitude;
            }
        }

        _AfterSpikes = true;
        _AfterSpikesInfo = true;
        _NrQRS = 0;
        delete[] _QRSType;
        _QRSType = null;
        _AfterQRSType = true;

        if (_ExtraMeasurements != null) {
            _ExtraMeasurements->VentRate = mes.getVentRate();
            _ExtraMeasurements->QTc = mes.getQTc();
            uchar temp = (uchar)(mes.getQTcType() + 1);

            if (temp > 2) {
                temp = 0;
            }

            _ExtraMeasurements->FormulaType = temp;
        }

        return 0;
    }

    return 1;
}
}
}
