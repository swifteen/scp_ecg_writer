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
    SCPStatement(uchar seqnr, ushort length, uchar field[])
    {
        SequenceNr = seqnr;
        Length = length;
        Field = field;
    }

    /// <summary>
    /// Function to write SCP statement.
    /// </summary>
    /// <param name="buffer">uchar array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar buffer[],int bufferLength, int offset)
    {
        if ((Field == null))
              //  ||  (Field.Length != Length))
        {
            return 0x1;
        }

        if ((offset + sizeof(SequenceNr) + sizeof(Length)) > bufferLength)
        {
            return 0x2;
        }

        BytesTool::writeBytes(SequenceNr, buffer,bufferLength, offset, sizeof(SequenceNr), true);

        offset += sizeof(SequenceNr);
        BytesTool::writeBytes(Length, buffer, bufferLength,offset, sizeof(Length), true);
        offset +=sizeof(Length);

        if ((offset + Length) > bufferLength)
        {
            return 0x2;
        }

        if (Length > 0)
        {
           // offset += BytesTool::copy(buffer, bufferLength,offset, Field, 0, Length);TODO

        }

        return 0x0;
    }
    /// <summary>
    /// Function to get length of SCP statement in uchars.
    /// </summary>
    /// <returns>length of scp statements</returns>
    int getLength()
    {
        int sum = sizeof(SequenceNr) + sizeof(Length);
        if ((Length > 0)
                &&	(Field != null))
               // &&  (Length == Field.Length))
        {
            sum += Length;
        }
        return sum;
    }
public:
    uchar SequenceNr;
    ushort Length;
    uchar *Field;
};

ushort SCPSection8::_SectionID = 8;
SCPSection8::SCPSection8()
{
    // Part of the stored Data Structure.
    uchar _Confirmed = 0;
  
    
    uchar _NrStatements = 0;

	 
   // SCPStatement[] _Statements = null;
}

int SCPSection8::_Write(uchar buffer[],int bufferLength, int offset)
{
    BytesTool::writeBytes(_Confirmed, buffer,bufferLength, offset, sizeof(_Confirmed), true);
    offset +=sizeof(_Confirmed);
    _Date.Write(buffer,bufferLength,offset);
    offset += _Date.Size;
    _Time.Write(buffer, bufferLength,offset);
    offset += _Time.Size;
    BytesTool::writeBytes(_NrStatements, buffer,bufferLength, offset, sizeof(_NrStatements), true);
    offset += sizeof(_NrStatements);
	_Statements.resize(_NrStatements);
    for (int loper=0;loper < _NrStatements;loper++)
    {
        _Statements[loper].Write(buffer, bufferLength,offset);
        offset += _Statements[loper].getLength();
    }
    return 0x0;
}

void SCPSection8::_Empty()
{
    _Confirmed = 0;
  //  _Date =  null;
    // _Time = null;
    _NrStatements = 0;
   // _Statements = null;
}

int SCPSection8::_getLength()
{
 
    if (Works())
    {
        int sum =sizeof(_Confirmed) + _Date.Size + _Time.Size + sizeof(_NrStatements);
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


	if (((_NrStatements == 0)
                 ||	 ((_Statements.size()!=0)
                      &&	  (_NrStatements <= _Statements.size()))))
    {
     
        return true;
    }
   
    
    return false;
}



/*

// getting diagnositc information.
int SCPSection8::getDiagnosticStatements(Statements stat)
{
   // stat = null;
    if (  (_NrStatements > 0))
    {
      //  stat = new Statements();
        stat.confirmed = (_Confirmed == 1);

        if ((_Date.Year != 0)
                &&	(_Date.Month != 0)
                &&	(_Date.Day != 0))
        	{
           // stat.time = new DateTime(_Date.Year, _Date.Month, _Date.Day, _Time.Hour, _Time.Min, _Time.Sec);
                	}
        else
        	{
          //  stat.time = DateTime.MinValue;
        	}

        //stat.statement = new string[_NrStatements];

		for (int loper=0;loper < _NrStatements;loper++)
        {
            if (
                      (_Statements[loper].Field != null)
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
}*/

/*

// setting diagnositc information.
int SCPSection8::setDiagnosticStatements(Statements stat)
{
    if ((stat != null)
            &&  (stat.time.Year > 1000)
            &&  (stat.statement != null)
            &&  (stat.statement.Length > 0))
    {
        Empty();
        _Confirmed = (uchar) (stat.confirmed ? 1 : 0);

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

        _NrStatements = (uchar) stat.statement.Length;
        _Statements = new SCPStatement[_NrStatements];
        for (int loper=0;loper < _NrStatements;loper++)
        {
            _Statements[loper] = new SCPStatement();
            _Statements[loper].SequenceNr = (uchar) (loper + 1);
            if (stat.statement[loper] != null)
            {
                _Statements[loper].Length = (ushort) (stat.statement[loper].Length + 1);
                _Statements[loper].Field = new uchar[_Statements[loper].Length];
                BytesTool::writeString(_Encoding, stat.statement[loper], _Statements[loper].Field, 0, _Statements[loper].Length);
            }
            else
            {
                _Statements[loper].Length = 1;
                _Statements[loper].Field = new uchar[_Statements[loper].Length];
            }
        }
        return 0;
    }
    return 1;
}
*/
}
}
