#include "SCPSection11.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// class for Universal ECG interpretive statements from SCP.
/// </summary>
class SCPSection11::SCPStatement
{
public:
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    SCPStatement()
    {
        SequenceNr = 0;
        TypeID = 0;
        Length = 0;
        Field = null;
    }
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    /// <param name="seqnr">sequence number of statement</param>
    /// <param name="length">length of byte array</param>
    /// <param name="field">byte array</param>
    SCPStatement(uchar seqnr, ushort length, uchar* field)
    {
        SequenceNr = seqnr;
        Length = length;
        if(length > 0)
        {
            Field = new uchar[length];
            memcpy(Field,field,length*sizeof(uchar));
        }
        else
        {
            Field = null;
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
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if ((Field == null))
        {
            return 0x1;
        }

        if ((offset + sizeof(SequenceNr) + sizeof(Length)) > bufferLength)
        {
            return 0x2;
        }

        BytesTool::writeBytes(SequenceNr, buffer, bufferLength,offset, sizeof(SequenceNr), true);
        offset += sizeof(SequenceNr);
        BytesTool::writeBytes(Length, buffer, bufferLength,offset, sizeof(Length), true);
        offset += sizeof(Length);

        if (Length >= sizeof(TypeID))
        {
            if ((offset + Length) > bufferLength)
            {
                return 0x2;
            }

            BytesTool::writeBytes(TypeID, buffer,bufferLength, offset, sizeof(TypeID), true);
            offset += sizeof(TypeID);

            if (Length > sizeof(TypeID))
            {
                offset += BytesTool::copy(buffer,
                                          bufferLength,
                                          offset,
                                          Field,
                                          Length - sizeof(TypeID),
                                          0,
                                          Length - sizeof(TypeID));
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
                &&	(Field != null))
        {
            sum += Length;
        }
        return sum;
    }
public:
    uchar SequenceNr;
    ushort Length;
    uchar TypeID;
    uchar* Field;
};

// Defined in SCP.
ushort SCPSection11::_SectionID = 11;

/// <summary>
/// Class contains section 11 (contains the Universal ECG interpretive statements section).
/// </summary>
SCPSection11::SCPSection11()
{
    // Part of the stored Data Structure.
    _Confirmed = 0;
    _NrStatements = 0;
    _Statements.clear();
}

int SCPSection11::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_Confirmed, buffer,bufferLength, offset, sizeof(_Confirmed), true);
    offset += sizeof(_Confirmed);
    _Date.Write(buffer,bufferLength, offset);
    offset += SCPDate::Size;
    _Time.Write(buffer, bufferLength,offset);
    offset += SCPTime::Size;
    BytesTool::writeBytes(_NrStatements, buffer,bufferLength, offset, sizeof(_NrStatements), true);
    offset += sizeof(_NrStatements);
    for (int loper=0;loper < _NrStatements;loper++)
    {
        _Statements[loper].Write(buffer, bufferLength,offset);
        offset += _Statements[loper].getLength();
    }
    return 0x0;
}
void SCPSection11::_Empty()
{
    _Confirmed = 0;
    _NrStatements = 0;
    _Statements.clear();
}
int SCPSection11::_getLength()
{
    if (Works())
    {
        int sum = sizeof(_Confirmed) + SCPDate::Size + SCPTime::Size + sizeof(_NrStatements);
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
    if ((_NrStatements == 0)
            ||	 (_NrStatements <= _Statements.size()))
    {
        return true;
    }
    return false;
}
}
}
