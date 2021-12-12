#include "SCPSection8.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGDiagnostic;	

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
    SCPStatement(uchar seqnr, ushort length, uchar* field)
    {
        SequenceNr = seqnr;
        Length = length;
        Field = field;
		if(length > 0)
		{
			Field = new uchar[length];
			if(Field != null)
			{
				memcpy(Field,field,length);
			}
		}
    }
    SCPStatement(const SCPStatement& rhs)
    {
		deepCopy(rhs);
    }
	
    SCPStatement& operator=(const SCPStatement& rhs)
    {
        // Prevent self-assignment
        if( &rhs != this )
        {
			delete[] this->Field;
			this->Field = null;
			deepCopy(rhs);
        }
        return *this;
    }
	
    ~SCPStatement()
    {
		delete[] Field;
    }
	
	void deepCopy(const SCPStatement& rhs)
    {
		this->Length = rhs.Length;
		if((rhs.Length > 0) && (rhs.Field != null))
		{
			this->Field = new uchar[rhs.Length];
			if(this->Field != null)
			{
				memcpy(this->Field,rhs.Field,rhs.Length);
			}
		}
	}

    /// <summary>
    /// Function to write SCP statement.
    /// </summary>
    /// <param name="buffer">uchar array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer,int bufferLength, int offset)
    {
        if (Field == null)
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
           offset += BytesTool::copy(buffer, bufferLength,offset, Field, Length,0, Length);
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
	_Confirmed = 0;    
	_NrStatements = 0;	 
	_Statements.clear();
}

int SCPSection8::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_Confirmed, buffer,bufferLength, offset, sizeof(_Confirmed), true);
    offset +=sizeof(_Confirmed);
    _Date.Write(buffer,bufferLength,offset);
    offset += SCPDate::Size;
    _Time.Write(buffer, bufferLength,offset);
    offset += SCPTime::Size;
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
    _NrStatements = 0;
	_Statements.clear();
}

int SCPSection8::_getLength()
{ 
    if (Works())
    {
        int sum =sizeof(_Confirmed) + SCPTime::Size + SCPTime::Size + sizeof(_NrStatements);
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
         ||	 ((_Statements.size() != 0)  && (_NrStatements <= _Statements.size()))))
    {     
        return true;
    }
       
    return false;
}

// setting diagnositc information.
int SCPSection8::setDiagnosticStatements(Statements& stat)
{
    if ((stat.time.Year > 1000)
        &&  (stat.statement.size() > 0))
    {
        Empty();
        _Confirmed = (uchar) (stat.confirmed ? 1 : 0);

        _Date = SCPDate(stat.time.Year, stat.time.Month, stat.time.Day);
        _Time = SCPTime(stat.time.Hour, stat.time.Minute, stat.time.Second);

        _NrStatements = (uchar) stat.statement.size();
		_Statements.resize(_NrStatements);
        for (int loper=0;loper < _NrStatements;loper++)
        {
            _Statements[loper].SequenceNr = (uchar) (loper + 1);
            if (stat.statement[loper].length() > 0)
            {
                _Statements[loper].Length = (ushort) (stat.statement[loper].length() + 1);
                _Statements[loper].Field = new uchar[_Statements[loper].Length];
#if 0//todo
                BytesTool::writeString(_Encoding, stat.statement[loper], _Statements[loper].Field, 0, _Statements[loper].Length);
#endif
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
}
}
