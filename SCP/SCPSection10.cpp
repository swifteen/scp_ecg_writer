#include "SCPSection10.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;


namespace ECGConversion.SCP
{
/// <summary>
/// class for Lead Measurements from SCP.
/// </summary>
class SCPSection10::SCPLeadMeasurements
{
public:
    void setLeadType(LeadType LeadId)
    {
        _LeadId = (ushort) LeadId;
    }
    void setCount(int Count)
    {
        int temp = (Count < 50) ? 50 : (Count << 1);

        if ((temp <= ushort.MaxValue)
                &&	(temp >= ushort.MinValue))
            _LeadLength = (ushort) temp;
    }
#if 0
    public short this[MeasurementType mt]
    {
    this[(int)mt] = value;
}
    public short this[int id]
    {
    if ((_Measurements != null)
    &&	(id >= 0)
    &&	(id < _Measurements.Length))
    {
    _Measurements[id] = value;
}
}
#endif

    void setLeadLength(ushort LeadLength)
    {
    if (LeadLength >> 1 < 50)
    LeadLength = 50 << 1;

    _Measurements = ((LeadLength == 0) && ((LeadLength & 0x1) != 0x1)) ? null : new short[LeadLength >> 1];

    if (_Measurements != null)
    {
        for (int i=0;i < _Measurements.Length;i++)
        {
            MeasurementType mt = (MeasurementType) i;

            bool bZero =	(mt == MeasurementType.Pmorphology)
                    ||	(mt == MeasurementType.Tmorphology)
                    ||	(mt == MeasurementType.QualityCode)
                    ||	(mt > MeasurementType.STamp1_8RR);

            _Measurements[i] = bZero ? (short) 0 : LeadMeasurement.NoValue;
        }
    }
}
/// <summary>
/// Constructor to make a SCP statement.
/// </summary>
SCPLeadMeasurements()
{}
/// <summary>
/// Constructor to make a SCP statement.
/// </summary>
SCPLeadMeasurements(LeadType lt, ushort nrMes)
{
    LeadId = lt;
    Count = nrMes;
}

/// <summary>
/// Function to write SCP statement.
/// </summary>
/// <param name="buffer">byte array to write into</param>
/// <param name="offset">position to start writing</param>
/// <returns>0 on success</returns>
int Write(byte[] buffer, int offset)
{
    if (!Works())
        return 0x1;

    if ((offset + getLength()) > buffer.Length)
        return 0x2;

    int fieldSize = sizeof(_LeadId);
    BytesTool::writeBytes(_LeadId, buffer, offset, fieldSize, true);
    offset += fieldSize;

    fieldSize = sizeof(_LeadLength);
    BytesTool::writeBytes(_LeadLength, buffer, offset, fieldSize, true);
    offset += fieldSize;

    if (_LeadLength != 0)
    {
        fieldSize = sizeof(typeof(short));

        for (int i=0;i < _Measurements.Length;i++)
        {
            BytesTool::writeBytes(_Measurements[i], buffer, offset, fieldSize, true);
            offset += fieldSize;
        }
    }

    return 0x0;
}
/// <summary>
/// Function to get length of SCP statement.
/// </summary>
/// <returns>length of statement</returns>
int getLength()
{
    if (Works())
    {
        int sum = sizeof(_LeadId) + sizeof(_LeadLength);

        if (_Measurements != null)
            sum += _LeadLength;

        return sum;
    }
    return 0;
}
bool Works()
{
    return _Measurements != null;
}
private:		
short[] _Measurements;
ushort _LeadId;
ushort _LeadLength
};

// Defined in SCP.
ushort SCPSection10::_SectionID = 10;
/// <summary>
/// Class contains section 10 (contains the Lead Measurements Result section).
/// </summary>
SCPSection10::SCPSection10()
{
    Empty();
}
void SCPSection10::setLeadMeasurements(ushort _NrLeads)
{
    _LeadMeasurements = (_NrLeads == 0) ? null : new SCPLeadMeasurements[value];
}

int SCPSection10::_Write(byte[] buffer, int offset)
{
    if (!Works())
        return 0x1;

    if ((offset + _getLength()) > buffer.Length)
        return 0x2;

    if (_NrLeads  == 0)
        return 0;

    int fieldSize = sizeof(_NrLeads);
    BytesTool::writeBytes(_NrLeads, buffer, offset, fieldSize, true);
    offset += fieldSize;

    fieldSize = sizeof(_ManufactorSpecific);
    BytesTool::writeBytes(_ManufactorSpecific, buffer, offset, fieldSize, true);
    offset += fieldSize;

    if (_NrLeads == 0)
        return 0x0;

    for (int loper=0;loper < _LeadMeasurements.Length;loper++)
    {
        int ret = _LeadMeasurements[loper].Write(buffer, offset);

        if (ret != 0)
            return 0x1;

        offset += _LeadMeasurements[loper].getLength();
    }
    return 0x0;
}
void SCPSection10::_Empty()
{
    _NrLeads = 0;
    _ManufactorSpecific = 0;
}
int SCPSection10::_getLength()
{
    if (Works())
    {
        if (_NrLeads == 0)
            return 0;

        int sum = sizeof(_NrLeads) + sizeof(_ManufactorSpecific);

        if (_NrLeads != 0)
        {
            for (int loper=0;loper < _LeadMeasurements.Length;loper++)
            {
                sum += _LeadMeasurements[loper].getLength();
            }
        }

        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}
ushort SCPSection10::getSectionID()
{
    return _SectionID;
}
bool SCPSection10::Works()
{
    if (_NrLeads == 0)
        return true;

    for (int loper=0;loper < _LeadMeasurements.Length;loper++)
    {
        if (_LeadMeasurements[loper] == null
                ||	!_LeadMeasurements[loper].Works())
        {
            return false;
        }
    }
    return true;
}

//region ILeadMeasurement Members
int SCPSection10::getLeadMeasurements(out LeadMeasurements mes)
{
    mes = null;

    if (_NrLeads != 0)
    {
        int nrLeads = _NrLeads;

        mes = new LeadMeasurements(nrLeads);

        for (int i=0;i < nrLeads;i++)
        {
            mes.Measurements[i].LeadType = _LeadMeasurements[i].LeadId;

            int len = _LeadMeasurements[i].Count;

            for (int j=0;j < len;j++)
                mes.Measurements[i][(MeasurementType) j] = _LeadMeasurements[i][j];
        }

        return 0;
    }

    return 1;
}

int SCPSection10::setLeadMeasurements(LeadMeasurements mes)
{
    if (mes != null)
    {
        int nrLeads = mes.Measurements.Length;

        _NrLeads = (ushort) nrLeads;

        for (int i=0;i < nrLeads;i++)
        {
            int nrValues = mes.Measurements[i].Count;

            nrValues = (nrLeads > 0) ? ((int) mes.Measurements[i].getKeyByIndex(nrValues-1))+1 : 0;

            _LeadMeasurements[i] = new SCPLeadMeasurements();
            _LeadMeasurements[i].LeadId = mes.Measurements[i].LeadType;
            _LeadMeasurements[i].Count = nrValues;

            nrValues = mes.Measurements[i].Count;

            for (int j=0;j < nrValues;j++)
                _LeadMeasurements[i][mes.Measurements[i].getKeyByIndex(j)] = mes.Measurements[i].getValueByIndex(j);
        }

        return 0;
    }

    return 1;
}
}
