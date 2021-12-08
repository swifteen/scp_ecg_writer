#include "SCPSection7.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication.IO.Tools;

namespace ECGConversion.SCP
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
        // Tagged Field are not implemented.
        TaggedFields = null;
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
    int Write(byte[] buffer, int offset)
    {
        BytesTool::writeBytes(VentRate, buffer, offset,sizeof(VentRate), true);
        offset +=sizeof(VentRate);
        BytesTool::writeBytes(AtrialRate, buffer, offset,sizeof(AtrialRate), true);
        offset +=sizeof(AtrialRate);
        BytesTool::writeBytes(QTc, buffer, offset,sizeof(QTc), true);
        offset +=sizeof(QTc);
        BytesTool::writeBytes(FormulaType, buffer, offset,sizeof(FormulaType), true);
        offset +=sizeof(FormulaType);
        BytesTool::writeBytes(TaggedFieldSize, buffer, offset,sizeof(TaggedFieldSize), true);
        offset +=sizeof(TaggedFieldSize);

        if (TaggedFields != null)
        {
            offset += BytesTool::copy(buffer, offset, TaggedFields, 0, TaggedFieldSize);
        }

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
        TaggedFields = null;
    }
    /// <summary>
    /// Function to get length of extra measurements.
    /// </summary>
    /// <returns>length of extra measurements</returns>
    int getLength()
    {
        if (Works())
        {
            int sum =sizeof(VentRate) +sizeof(AtrialRate);
            sum +=sizeof(QTc) +sizeof(FormulaType) +sizeof(TaggedFieldSize);
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
        if (TaggedFieldSize == 0)
        {
            return true;
        }
        else if ((TaggedFields != null)
                 &&   (TaggedFields.Length >= TaggedFieldSize))
        {
            return true;
        }
        return false;
    }
public:
    ushort VentRate ;
    ushort AtrialRate;
    ushort QTc;
    byte FormulaType;
    ushort TaggedFieldSize;
    // Tagged Field are not implemented.
    byte[] TaggedFields;
};


/// <summary>
/// Class containing SCP Spike info.
/// </summary>
class SCPSection7::SCPSpikeInfo
{
    public：
    SCPSpikeInfo():Size(9)
    {}
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
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Type, buffer, offset,sizeof(Type), true);
        offset +=sizeof(Type);
        BytesTool::writeBytes(Source, buffer, offset,sizeof(Source), true);
        offset +=sizeof(Source);
        BytesTool::writeBytes(TriggerIndex, buffer, offset,sizeof(TriggerIndex), true);
        offset +=sizeof(TriggerIndex);
        BytesTool::writeBytes(PulseWidth, buffer, offset,sizeof(PulseWidth), true);
        offset +=sizeof(PulseWidth);

        return 0x0;
    }

    const int Size ;
    byte Type;
    byte Source;
    ushort TriggerIndex ;
    ushort PulseWidth;
};

/// <summary>
/// Class containing measurements for SCP.
/// </summary>
class SCPSection7::SCPMeasurement
{
public:
    SCPMeasurement():Size(16)
    {}
   	SCPMeasurement()
    {
        Ponset = GlobalMeasurement.NoValue;
        Poffset = GlobalMeasurement.NoValue;
        QRSonset = GlobalMeasurement.NoValue;
        QRSoffset = GlobalMeasurement.NoValue;
        Toffset = GlobalMeasurement.NoValue;
        Paxis = GlobalMeasurement.NoAxisValue;
        QRSaxis = GlobalMeasurement.NoAxisValue;
        Taxis = GlobalMeasurement.NoAxisValue;
    }

    /// <summary>
    /// Function to write measurements.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Ponset, buffer, offset,sizeof(Ponset), true);
        offset +=sizeof(Ponset);
        BytesTool::writeBytes(Poffset, buffer, offset,sizeof(Poffset), true);
        offset +=sizeof(Poffset);
        BytesTool::writeBytes(QRSonset, buffer, offset,sizeof(QRSonset), true);
        offset +=sizeof(QRSonset);
        BytesTool::writeBytes(QRSoffset, buffer, offset,sizeof(QRSoffset), true);
        offset +=sizeof(QRSoffset);
        BytesTool::writeBytes(Toffset, buffer, offset,sizeof(Toffset), true);
        offset +=sizeof(Toffset);
        BytesTool::writeBytes(Paxis, buffer, offset,sizeof(Paxis), true);
        offset +=sizeof(Paxis);
        BytesTool::writeBytes(QRSaxis, buffer, offset,sizeof(QRSaxis), true);
        offset +=sizeof(QRSaxis);
        BytesTool::writeBytes(Taxis, buffer, offset,sizeof(Taxis), true);
        offset +=sizeof(Taxis);

        return 0x0;
    }
public:
    const int Size ;
    ushort Ponset ;
    ushort Poffset ;
    ushort QRSonset ;
    ushort QRSoffset ;
    ushort Toffset ;
    short Paxis ;
    short QRSaxis;
    short Taxis ;
};

/// <summary>
/// Class containing SCP spikes.
/// </summary>
class SCPSection7::SCPSpike
{
public:
    SCPSpike():Size(4)
    {}
    SCPSpike()
    {
        Time = GlobalMeasurement.NoValue;
        Amplitude = GlobalMeasurement.NoAxisValue;
    }
    /// <summary>
    /// Function to write SCP spike.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Time, buffer, offset,sizeof(Time), true);
        offset +=sizeof(Time);
        BytesTool::writeBytes(Amplitude, buffer, offset,sizeof(Amplitude), true);
        offset +=sizeof(Amplitude);

        return 0x0;
    }
public:
    const int Size;
    ushort Time;
    short Amplitude;
};

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
    _Measurements = null;
    _Spikes = null;
    _SpikesInfo = null;
    _NrQRS = 0;
    _QRSType = null;
    _ExtraMeasurements = new SCPExtraMeasurements();

    // Manufactor specific block (Not implemented, because UNIPRO doesn't store this kind of info).
    _Rest = null;
}

int SCPSection7::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_NrRefTypeQRS, buffer, offset,sizeof(_NrRefTypeQRS), true);
    offset +=sizeof(_NrRefTypeQRS);
    BytesTool::writeBytes(_NrSpikes, buffer, offset,sizeof(_NrSpikes), true);
    offset +=sizeof(_NrSpikes);
    BytesTool::writeBytes(_AvgRRInterval, buffer, offset,sizeof(_AvgRRInterval), true);
    offset +=sizeof(_AvgRRInterval);
    BytesTool::writeBytes(_AvgPPInterval, buffer, offset,sizeof(_AvgPPInterval), true);
    offset +=sizeof(_AvgPPInterval);

    if (_NrRefTypeQRS > 0)
    {
        for (int loper=0;loper < _NrRefTypeQRS;loper++)
        {
            int err = _Measurements[loper].Write(buffer, offset);
            if (err != 0)
            {
                return 0x1;
            }
            offset += SCPMeasurement.Size;
        }
    }
    if (_NrSpikes > 0)
    {
        for (int loper=0;loper < _NrSpikes;loper++)
        {
            int err = _Spikes[loper].Write(buffer, offset);
            if (err != 0)
            {
                return 0x2;
            }
            offset += SCPSpike.Size;
        }
        if (!_AfterSpikes)
        {
            return 0;
        }
        for (int loper=0;loper < _NrSpikes;loper++)
        {
            int err = _SpikesInfo[loper].Write(buffer, offset);
            if (err != 0)
            {
                return 0x4;
            }
            offset += SCPSpikeInfo.Size;
        }
    }

    if (!_AfterSpikesInfo)
    {
        return 0;
    }

    BytesTool::writeBytes(_NrQRS, buffer, offset,sizeof(_NrQRS), true);
    offset +=sizeof(_NrQRS);

    if ((_NrQRS > 0)
            &&  (_QRSType != null))
    {
        for (int loper=0;loper < _NrQRS;loper++)
        {
            BytesTool::writeBytes(_QRSType[loper], buffer, offset,sizeof(_QRSType[loper]), true);
            offset +=sizeof(_QRSType[loper]);
        }
    }

    if (!_AfterQRSType)
    {
        return 0;
    }

    if (_ExtraMeasurements != null)
    {
        int err = _ExtraMeasurements.Write(buffer, offset);
        if (err != 0)
        {
            return 0x8;
        }
        offset += _ExtraMeasurements.getLength();
    }

    if ((_Rest != null)
            &&  ((offset + _Rest.Length) < buffer.Length))
    {
        offset += BytesTool::copy(_Rest, 0, buffer, offset, _Rest.Length);
    }
    return 0x0;
}

void SCPSection7::_Empty()
{
    _NrRefTypeQRS = 0;
    _NrSpikes = 0;
    _AvgRRInterval = 0;
    _AvgPPInterval = 0;
    _Measurements = null;
    _Spikes = null;
    _SpikesInfo = null;
    _AfterSpikes = false;
    _NrQRS = 0;
    _AfterQRSType = false;
    _QRSType = null;
    _ExtraMeasurements.Empty();
    _Rest = null;
}

int SCPSection7::_getLength()
{
    if (Works())
    {
        int sum =sizeof(_NrRefTypeQRS) +sizeof(_NrSpikes) +sizeof(_AvgPPInterval) +sizeof(_AvgRRInterval);
        sum += _NrRefTypeQRS * SCPMeasurement.Size;
        sum += (_NrSpikes * (SCPSpike.Size + SCPSpikeInfo.Size));
        if (_AfterSpikes)
        {
            sum +=sizeof(_NrQRS) + (_NrQRS *sizeof(typeof(byte)));
            if (_AfterQRSType)
            {
                sum += _ExtraMeasurements.getLength();
                if (_Rest != null)
                {
                    sum += _Rest.Length;
                }
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
            ||	((_Measurements != null)
                 &&	 (_Measurements.Length == _NrRefTypeQRS)))
    {
        if ((_NrSpikes == 0)
                ||  ((_Spikes != null)
                     &&	 (_Spikes.Length == _NrSpikes)))
        {
            if (!_AfterSpikes)
            {
                return ((_NrRefTypeQRS != 0) || (_NrSpikes != 0));
            }

            if ((_SpikesInfo != null)
                    &&  (_SpikesInfo.Length == _NrSpikes))
            {
                if (!_AfterSpikesInfo)
                {
                    return ((_NrRefTypeQRS != 0) || (_NrSpikes != 0));
                }

                if ((_NrQRS == 0)
                        ||  ((_QRSType != null)
                             &&	 (_QRSType.Length == _NrQRS)))
                {
                    return (!_AfterQRSType
                            ||	(_ExtraMeasurements != null));
                }
            }
        }
        return true;
    }
    return false;
}

int SCPSection7::setGlobalMeasurements(GlobalMeasurements mes)
{
    if ((mes != null)
            &&  (mes.measurment != null))
    {
        Empty();

        _AvgRRInterval = mes.AvgRR;
        _AvgPPInterval = mes.AvgPP;

        _NrRefTypeQRS = (byte) mes.measurment.Length;
        _Measurements = new SCPMeasurement[_NrRefTypeQRS];
        for (int loper=0;loper < _NrRefTypeQRS;loper++)
        {
            _Measurements[loper] = new SCPMeasurement();
            if (mes.measurment[loper] != null)
            {
                _Measurements[loper].Ponset = mes.measurment[loper].Ponset;
                _Measurements[loper].Poffset = mes.measurment[loper].Poffset;
                _Measurements[loper].QRSonset = mes.measurment[loper].QRSonset;
                _Measurements[loper].QRSoffset = mes.measurment[loper].QRSoffset;
                _Measurements[loper].Toffset = mes.measurment[loper].Toffset;
                _Measurements[loper].Paxis = mes.measurment[loper].Paxis;
                _Measurements[loper].QRSaxis = mes.measurment[loper].QRSaxis;
                _Measurements[loper].Taxis = mes.measurment[loper].Taxis;
            }
        }

        _NrSpikes = 0;

        if (mes.spike != null)
        {
            _NrSpikes = (byte) mes.spike.Length;
            _Spikes = new SCPSpike[_NrSpikes];
            _SpikesInfo = new SCPSpikeInfo[_NrSpikes];
            for (int loper=0;loper < _NrSpikes;loper++)
            {
                _Spikes[loper] = new SCPSpike();
                _SpikesInfo[loper] = new SCPSpikeInfo();
                if (mes.spike[loper] != null)
                {
                    _Spikes[loper].Time = mes.spike[loper].Time;
                    _Spikes[loper].Amplitude = mes.spike[loper].Amplitude;
                }
            }
        }

        _AfterSpikes = true;

        _AfterSpikesInfo = true;

        _NrQRS = 0;
        _QRSType = null;

        _AfterQRSType = true;

        _ExtraMeasurements = new SCPExtraMeasurements();
        _ExtraMeasurements.VentRate = mes.VentRate;
        _ExtraMeasurements.QTc = mes.QTc;

        byte temp = (byte) (mes.QTcType + 1);

        if (temp > 2)
            temp = 0;

        _ExtraMeasurements.FormulaType = temp;

        return 0;
    }
    return 1;
}

}
