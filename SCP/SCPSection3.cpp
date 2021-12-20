#include "SCPSection3.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGSignals;

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
    SCPLead()
    {
        Start = 0;
        End = 0;
        ID = 0;
    }
    /// <summary>
    /// Constructor of SCP lead.
    /// </summary>
    /// <param name="start">start sample of lead</param>
    /// <param name="end">end sample of lead</param>
    /// <param name="id">id of lead</param>
    SCPLead(int start, int end, uchar id)
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
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if ((offset + Size) > bufferLength) {
            return 0x1;
        }

        BytesTool::writeBytes(Start, buffer, bufferLength, offset, sizeof(Start), true);
        offset += sizeof(Start);
        BytesTool::writeBytes(End, buffer, bufferLength, offset, sizeof(End), true);
        offset += sizeof(End);
        BytesTool::writeBytes(ID, buffer, bufferLength, offset, sizeof(ID), true);
        offset += sizeof(ID);
        return 0x0;
    }
public:
    static int Size;

    int Start;
    int End;
    uchar ID;
};

int SCPSection3::SCPLead::Size = 9;

ushort SCPSection3::_SectionID = 3;
SCPSection3::SCPSection3()
{
    SCPSection::Empty();
    // Part of the stored Data Structure.
    _NrLeads = 0;
    _Flags = 0;
    _Flags |= 0x01;//Current :Reference beat subtraction not used for compression
    _Leads.clear();
}

int SCPSection3::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_NrLeads, buffer, bufferLength, offset, sizeof(_NrLeads), true);
    offset += sizeof(_NrLeads);
    BytesTool::writeBytes(_Flags, buffer, bufferLength, offset, sizeof(_Flags), true);
    offset += sizeof(_Flags);

    for (int loper = 0; loper < _NrLeads; loper++) {
        int err = _Leads[loper].Write(buffer, bufferLength, offset);

        if (err != 0) {
            return err << loper;
        }

        offset += SCPLead::Size;
    }

    return 0x0;
}
void SCPSection3::_Empty()
{
    _NrLeads = 0;
    _Flags = 0;
    _Flags |= 0x01;//Current :Reference beat subtraction not used for compression
}
int SCPSection3::_getLength()
{
    if (Works()) {
        int sum = (sizeof(_NrLeads) + sizeof(_Flags));
        sum += (_NrLeads * SCPLead::Size);
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
    if ((_Leads.size() > 0) && (_NrLeads == _Leads.size())) {
#if 0

        for (int loper = 0; loper < _NrLeads; loper++) {
            if (_Leads[loper] == null) {
                return false;
            }
        }

#endif
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
    if ((_Leads.size() > 0)
        && (nr >= 0)
        && (nr < _NrLeads)) {
        return _Leads[nr].Start;
    }

    return Int32_MaxValue;
}
/// <summary>
/// Function to get lead end.
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>end of lead</returns>
int SCPSection3::getLeadEnd(int nr)
{
    if ((_Leads.size() > 0)
        && (nr >= 0)
        && (nr < _NrLeads)) {
        return _Leads[nr].End;
    }

    return Int32_MinValue;
}
/// <summary>
/// Function to get id of lead.
/// </summary>
/// <param name="nr">number of lead</param>
/// <returns>id of lead</returns>
int SCPSection3::getLeadId(int nr)
{
    if ((_Leads.size() > 0)
        && (nr >= 0)
        && (nr < _NrLeads)) {
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

    if ((_Leads.size() > 0)
        && (nr >= 0)
        && (nr < _NrLeads)) {
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
    int min = Int32_MaxValue;

    if (Works()) {
        for (int x = 0; x < _NrLeads; x++) {
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
    int max = Int32_MinValue;

    if (Works()) {
        for (int x = 0; x < _NrLeads; x++) {
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
    if (used) {
        _Flags |= 0x01;
    }
    else {
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
    if ((_Leads.size() > 0)
        && (_NrLeads > 1)
        && (_NrLeads <= _Leads.size())) {
        int loper = 1;

        for (; loper < _NrLeads; loper++) {
            if ((_Leads[0].Start != _Leads[loper].Start)
                || (_Leads[0].End != _Leads[loper].End)) {
                break;
            }
        }

        _Flags |= (uchar)(loper << 3);
        return (loper == _NrLeads);
    }

    if ((_Leads.size() > 0) && (_NrLeads == 1)) {
        _Flags = 0x8;
    }

    return (_Leads.size() > 0) && (_NrLeads == 1);
}
int SCPSection3::setSignals(Signals& signals)
{
    if ((signals.getNrLeads() > 0)
        && (signals.RhythmSamplesPerSecond != 0)) {
        _NrLeads = (uchar) signals.getNrLeads();
        _Leads.resize(_NrLeads);
        _Flags = 0;
        _Flags |= 0x01;//Current :Reference beat subtraction not used for compression

        for (int loper = 0; loper < _NrLeads; loper++) {
#if 0//only support rhythm data

            if (signals.MedianSamplesPerSecond != 0) {
                _Leads[loper].Start = (signals[loper].RhythmStart * signals.MedianSamplesPerSecond) / signals.RhythmSamplesPerSecond + 1;
                _Leads[loper].End = (signals[loper].RhythmEnd * signals.MedianSamplesPerSecond) / signals.RhythmSamplesPerSecond;
            }
            else
#endif
            {
                _Leads[loper].Start = signals[loper].RhythmStart + 1;
                _Leads[loper].End = signals[loper].RhythmEnd;
            }

            _Leads[loper].ID = (uchar) signals[loper].Type;
        }

        if (_isSimultaneously()) {
            _Flags |= 0x4;
        }

        return 0;
    }

    return 1;
}
}
}
