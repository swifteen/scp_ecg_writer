#include "SCPSection3.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{

/// <summary>
/// Class containing SCP lead information.
/// </summary>
class SCPSection3::SCPLead
{
public:
    /// <summary>
    /// Constructor of SCP lead.
    /// </summary>
    SCPLead():Size(9)
    {}
    /// <summary>
    /// Constructor of SCP lead.
    /// </summary>
    /// <param name="start">start sample of lead</param>
    /// <param name="end">end sample of lead</param>
    /// <param name="id">id of lead</param>
    SCPLead(int start, int end, byte id)
    {
        Start = start;
        End = end;
        ID = id;
    }

    /// <summary>
    /// Function to write SCP lead information.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to write to</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Start, buffer, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(End, buffer, offset, sizeof(End), true);
        offset += sizeof(End);
        BytesTool::writeBytes(ID, buffer, offset,sizeof(ID), true);
        offset += sizeof(ID);

        return 0x0;
    }
public:

    const int Size;

    int Start;
    int End;
    byte ID;
};


ushort SCPSection3::_SectionID = 3;
SCPSection3::SCPSection3()
{


    // Part of the stored Data Structure.
    byte _NrLeads = 0;
    byte _Flags = 0;
    SCPLead[] _Leads = null;

}

int SCPSection3::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_NrLeads, buffer, offset,sizeof(_NrLeads), true);
    offset += sizeof(_NrLeads);
    BytesTool::writeBytes(_Flags, buffer, offset, sizeof(_Flags), true);
    offset += sizeof(_Flags);
    for (int loper=0;loper < _NrLeads;loper++)
    {
        int err = _Leads[loper].Write(buffer, offset);
        if (err != 0)
        {
            return err << loper;
        }
        offset += SCPLead.Size;
    }
    return 0x0;
}
void SCPSection3::_Empty()
{
    _NrLeads = 0;
    _Flags = 0;
}
int SCPSection3::_getLength()
{
    if (Works())
    {
        int sum = (sizeof(_NrLeads) + sizeof(this._Flags));
        sum += (_NrLeads * SCPLead.Size);
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}
ushort SCPSection3::getSectionID()
{
    return _SectionID;
}
bool SCPSection3::Works()
{
    if ((_Leads != null) && (_NrLeads == _Leads.Length))
    {
        for (int loper=0;loper < _NrLeads;loper++)
        {
            if (_Leads[loper] == null)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
/// <summary>
/// Function to get number of leads.
/// </summary>
/// <returns>number of leads.</returns>
ushort SCPSection3::getNrLeads()
{
    return _NrLeads;
}
/// <summary>
/// Function to get beginning of lead.
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>begin of lead</returns>
int SCPSection3::getLeadStart(int nr)
{
    if ((_Leads != null)
            &&  (nr >= 0)
            &&  (nr < _NrLeads))
    {
        return _Leads[nr].Start;
    }
    return Int32.MaxValue;
}
/// <summary>
/// Function to get lead end.
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>end of lead</returns>
int SCPSection3::getLeadEnd(int nr)
{
    if ((_Leads != null)
            &&  (nr >= 0)
            &&  (nr < _NrLeads))
    {
        return _Leads[nr].End;
    }
    return Int32.MinValue;
}
/// <summary>
/// Function to get id of lead.
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>id of lead</returns>
int SCPSection3::getLeadId(int nr)
{
    if ((_Leads != null)
            &&  (nr >= 0)
            &&  (nr < _NrLeads))
    {
        return _Leads[nr].ID;
    }
    return -1;
}
/// <summary>
/// Function to get length of lead
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>length of lead</returns>
int SCPSection3::getLeadLength(int nr)
{
    int length = -1;
    if ((_Leads != null)
            &&  (nr >= 0)
            &&  (nr < _NrLeads))
    {
        length = _Leads[nr].End - _Leads[nr].Start + 1;
    }
    return length;
}
/// <summary>
/// Function of minimum beginning of leads.
/// </summary>
/// <returns>min begin</returns>
int SCPSection3::getMinBegin()
{
    int min = Int32.MaxValue;
    if (Works())
    {
        for (int x=0;x < _NrLeads;x++)
        {
            min = (min <= getLeadStart(x) ? min : getLeadStart(x));
        }
    }
    return min;
}
/// <summary>
/// Function of maximum ending of leads
/// </summary>
/// <returns>max end</returns>
int SCPSection3::getMaxEnd()
{
    int max = Int32.MinValue;
    if (Works())
    {
        for (int x=0;x < _NrLeads;x++)
        {
            max = (max >= getLeadEnd(x) ? max : getLeadEnd(x));
        }
    }
    return max;
}
/// <summary>
/// Function to get total length of all the leads.
/// </summary>
/// <returns>total length</returns>
int SCPSection3::getTotalLength()
{
    return getMaxEnd() - getMinBegin();
}
/// <summary>
/// Function to determine if medians are used.
/// </summary>
/// <returns>true if medians are used</returns>
bool SCPSection3::isMediansUsed()
{
    return ((_Flags & 0x1) == 0x1);
}
/// <summary>
/// Function to set use of median subtraction.
/// </summary>
/// <param name="used">true if medians used</param>
void SCPSection3::setMediansUsed(bool used)
{
    if (used)
    {
        _Flags |= 0x01;
    }
    else
    {
        _Flags &= 0xfe;
    }
}
/// <summary>
/// Function to determine if all leads are simultaneously.
/// </summary>
/// <returns>true if leads are simultaneous</returns>
bool SCPSection3::isSimultaneously()
{
    return ((_Flags & 0x4) == 0x4);
}
/// <summary>
/// Function to determine if all leads are simultaneously.
/// </summary>
/// <returns>true if leads are simultaneous</returns>
bool SCPSection3::_isSimultaneously()
{
    if ((_Leads != null)
            &&	(_NrLeads > 1)
            &&  (_NrLeads <= _Leads.Length)
            &&  (_Leads[0] != null))
    {
        int loper=1;
        for (;loper < _NrLeads;loper++)
        {
            if ((_Leads[loper] == null)
                    ||	(_Leads[0].Start != _Leads[loper].Start)
                    ||  (_Leads[0].End != _Leads[loper].End))
            {
                break;
            }
        }
        _Flags |= (byte) (loper << 3);
        return (loper == _NrLeads);
    }
    if ((_Leads != null) && (_NrLeads == 1))
    {
        _Flags = 0x8;
    }
    return (_Leads != null) && (_NrLeads == 1);
}
// Signal Manupalations
int SCPSection3::getSignals(out Signals signals)
{
    signals = new Signals();
    int err = getSignalsToObj(signals);
    if (err != 0)
    {
        signals = null;
    }
    return err;
}
int SCPSection3::getSignalsToObj(Signals signals)
{
    if ((signals != null)
            &&  (Works()))
    {
        signals.NrLeads = _NrLeads;

        for (int loper=0;loper < _NrLeads;loper++)
        {
            signals[loper] = new Signal();
            signals[loper].Type = (LeadType) _Leads[loper].ID;
            signals[loper].RhythmStart = _Leads[loper].Start - 1;
            signals[loper].RhythmEnd = _Leads[loper].End;
        }

        return 0;
    }
    return 1;
}
int SCPSection3::setSignals(Signals signals)
{
    if ((signals != null)
            &&  (signals.NrLeads > 0)
            &&  (signals.RhythmSamplesPerSecond != 0))
    {
        _NrLeads = (byte) signals.NrLeads;
        _Leads = new SCPLead[_NrLeads];
        _Flags = 0;
        for (int loper=0;loper< _NrLeads;loper++)
        {
            if (signals[loper] == null)
            {
                return 2;
            }

            _Leads[loper] = new SCPLead();
            if (signals.MedianSamplesPerSecond != 0)
            {
                _Leads[loper].Start = (signals[loper].RhythmStart * signals.MedianSamplesPerSecond) / signals.RhythmSamplesPerSecond + 1;
                _Leads[loper].End = (signals[loper].RhythmEnd * signals.MedianSamplesPerSecond) / signals.RhythmSamplesPerSecond;
            }
            else
            {
                _Leads[loper].Start = signals[loper].RhythmStart + 1;
                _Leads[loper].End = signals[loper].RhythmEnd;
            }
            _Leads[loper].ID = (byte) signals[loper].Type;
        }

        if (_isSimultaneously())
        {
            _Flags |= 0x4;
        }
        return 0;
    }
    return 1;
}
}
}
