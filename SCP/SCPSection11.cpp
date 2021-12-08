#include "SCPSection11.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication.IO.Tools;

namespace ECGConversion.SCP
{
/// <summary>
/// class for Universal ECG interpretive statements from SCP.
/// </summary>
class SCPSection11::SCPStatement
{
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    SCPStatement()
    {}
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    /// <param name="seqnr">sequence number of statement</param>
    /// <param name="length">length of byte array</param>
    /// <param name="field">byte array</param>
    SCPStatement(byte seqnr, ushort length, byte[] field)
    {
        SequenceNr = seqnr;
        Length = length;
        Field = field;
    }

    /// <summary>
    /// Function to write SCP statement.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {

        if ((Field == null)
                ||	(Field.Length != Length))
        {
            return 0x1;
        }

        if ((offset + sizeof(SequenceNr) + sizeof(Length)) > buffer.Length)
        {
            return 0x2;
        }

        BytesTool::writeBytes(SequenceNr, buffer, offset, sizeof(SequenceNr), true);
        offset += sizeof(SequenceNr);
        BytesTool::writeBytes(Length, buffer, offset, sizeof(Length), true);
        offset += sizeof(Length);

        if (Length >= sizeof(TypeID))
        {
            if ((offset + Length) > buffer.Length)
            {
                return 0x2;
            }

            BytesTool::writeBytes(TypeID, buffer, offset, sizeof(TypeID), true);
            offset += sizeof(TypeID);

            if (Length > sizeof(TypeID))
            {
                offset += BytesTool::copy(buffer, offset, Field, 0, Length - sizeof(TypeID));
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
        int sum = sizeof(SequenceNr) + sizeof(Length);
        if ((Length > 0)
                &&	(Field != null)
                &&	(Length == Field.Length))
        {
            sum += Length;
        }
        return sum;
    }
public:
    byte SequenceNr;
    ushort Length;
    byte TypeID;
    byte[] Field;
}

// Defined in SCP.
ushort SCPSection11::_SectionID = 11;

/// <summary>
/// Class contains section 11 (contains the Universal ECG interpretive statements section).
/// </summary>
SCPSection11::SCPSection11()
{
    // Part of the stored Data Structure.
    _Confirmed = 0;
    _Date = null;
    _Time = null;
    _NrStatements = 0;
    _Statements = null;
}

int SCPSection11::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_Confirmed, buffer, offset, sizeof(_Confirmed), true);
    offset += sizeof(_Confirmed);
    _Date.Write(buffer, offset);
    offset += SCPDate.Size;
    _Time.Write(buffer, offset);
    offset += SCPTime.Size;
    BytesTool::writeBytes(_NrStatements, buffer, offset, sizeof(_NrStatements), true);
    offset += sizeof(_NrStatements);
    for (int loper=0;loper < _NrStatements;loper++)
    {
        _Statements[loper].Write(buffer, offset);
        offset += _Statements[loper].getLength();
    }
    return 0x0;
}
void SCPSection11::_Empty()
{
    _Confirmed = 0;
    _Date = null;
    _Time = null;
    _NrStatements = 0;
    _Statements = null;
}
int SCPSection11::_getLength()
{
    if (Works())
    {
        int sum = sizeof(_Confirmed) + SCPDate.Size + SCPTime.Size + sizeof(_NrStatements);
        for (int loper=0;loper < _NrStatements;loper++)
        {
            sum += _Statements[loper].getLength();
        }
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}
ushort SCPSection11::getSectionID()
{
    return _SectionID;
}
bool SCPSection11::Works()
{
    if ((_Date != null)
            &&	(_Time != null)
            &&  ((_NrStatements == 0)
                 ||	 ((_Statements != null)
                      &&	  (_NrStatements <= _Statements.Length))))
    {
        for (int loper=0;loper < _NrStatements;loper++)
        {
            if (_Statements[loper] == null)
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
}
