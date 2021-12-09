#include "SCPSection8.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{

/// <summary>
/// Class containing SCP diagnostic statement.
/// </summary>
class SCPSection8::SCPStatement
{
public:
    /// <summary>
    /// Constructor for an SCP statement.
    /// </summary>
    SCPStatement()
    {}
    /// <summary>
    /// Constructor for an SCP statement.
    /// </summary>
    /// <param name="seqnr">sequence number</param>
    /// <param name="length">length of field</param>
    /// <param name="field">field to use</param>
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
                ||  (Field.Length != Length))
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
        offset +=sizeof(Length);

        if ((offset + Length) > buffer.Length)
        {
            return 0x2;
        }

        if (Length > 0)
        {
            offset += BytesTool::copy(buffer, offset, Field, 0, Length);
        }

        return 0x0;
    }
    /// <summary>
    /// Function to get length of SCP statement in bytes.
    /// </summary>
    /// <returns>length of scp statements</returns>
    int getLength()
    {
        int sum = sizeof(SequenceNr) + sizeof(Length);
        if ((Length > 0)
                &&	(Field != null)
                &&  (Length == Field.Length))
        {
            sum += Length;
        }
        return sum;
    }
public:
    byte SequenceNr;
    ushort Length;
    byte[] Field;
};

ushort SCPSection8::_SectionID = 8;
SCPSection8::SCPSection8()
{
    // Part of the stored Data Structure.
    byte _Confirmed = 0;
    SCPDate _Date = null;
    SCPTime _Time = null;
    byte _NrStatements = 0;
    SCPStatement[] _Statements = null;
}

int SCPSection8::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_Confirmed, buffer, offset, sizeof(_Confirmed), true);
    offset +=sizeof(_Confirmed);
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

void SCPSection8::_Empty()
{
    _Confirmed = 0;
    _Date = null;
    _Time = null;
    _NrStatements = 0;
    _Statements = null;
}

int SCPSection8::_getLength()
{
    if (Works())
    {
        int sum =sizeof(_Confirmed) + SCPDate.Size + SCPTime.Size + sizeof(_NrStatements);
        for (int loper=0;loper < _NrStatements;loper++)
        {
            sum += _Statements[loper].getLength();
        }
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}

ushort SCPSection8::getSectionID()
{
    return _SectionID;
}

bool SCPSection8::Works()
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

// getting diagnositc information.
int SCPSection8::getDiagnosticStatements(out Statements stat)
{
    stat = null;
    if ((_Date != null)
            &&	(_Time != null)
            &&	(_NrStatements > 0))
    {
        stat = new Statements();
        stat.confirmed = (_Confirmed == 1);

        if ((_Date.Year != 0)
                &&	(_Date.Month != 0)
                &&	(_Date.Day != 0))
            stat.time = new DateTime(_Date.Year, _Date.Month, _Date.Day, _Time.Hour, _Time.Min, _Time.Sec);
        else
            stat.time = DateTime.MinValue;

        stat.statement = new string[_NrStatements];
        for (int loper=0;loper < _NrStatements;loper++)
        {
            if ((_Statements[loper] != null)
                    &&  (_Statements[loper].Field != null)
                    &&  (_Statements[loper].Length <= _Statements[loper].Field.Length))
            {
                stat.statement[loper] = BytesTool::readString(_Encoding, _Statements[loper].Field, 0, _Statements[loper].Length);
            }
        }

        if  ((stat.statement.Length == 1)
             &&   ((stat.statement[0] == null)
                   ||	  (stat.statement[0].Length == 0)))
        {
            stat = null;

            return 1;
        }

        return 0;
    }
    return 1;
}

// setting diagnositc information.
int SCPSection8::setDiagnosticStatements(Statements stat)
{
    if ((stat != null)
            &&  (stat.time.Year > 1000)
            &&  (stat.statement != null)
            &&  (stat.statement.Length > 0))
    {
        Empty();
        _Confirmed = (byte) (stat.confirmed ? 1 : 0);

        if (stat.time == DateTime.MinValue)
        {
            _Date = new SCPDate();
            _Time = new SCPTime();
        }
        else
        {
            _Date = new SCPDate(stat.time.Year, stat.time.Month, stat.time.Day);
            _Time = new SCPTime(stat.time.Hour, stat.time.Minute, stat.time.Second);
        }

        _NrStatements = (byte) stat.statement.Length;
        _Statements = new SCPStatement[_NrStatements];
        for (int loper=0;loper < _NrStatements;loper++)
        {
            _Statements[loper] = new SCPStatement();
            _Statements[loper].SequenceNr = (byte) (loper + 1);
            if (stat.statement[loper] != null)
            {
                _Statements[loper].Length = (ushort) (stat.statement[loper].Length + 1);
                _Statements[loper].Field = new byte[_Statements[loper].Length];
                BytesTool::writeString(_Encoding, stat.statement[loper], _Statements[loper].Field, 0, _Statements[loper].Length);
            }
            else
            {
                _Statements[loper].Length = 1;
                _Statements[loper].Field = new byte[_Statements[loper].Length];
            }
        }
        return 0;
    }
    return 1;
}
}
}
