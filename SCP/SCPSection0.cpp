#include "SCPSection0.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion.SCP
{
/// <summary>
/// Class containing a SCP pointer.
/// </summary>
class SCPSection0::SCPPointer
{
public:
    SCPPointer():Size(10)//类的const变量在构造函数初始化列表中完成初始化
    {
    }
    /// <summary>
    /// Function to write SCP pointer.
    /// </summary>
    /// <param name="buffer">byte array to write section into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(Nr, buffer, offset, sizeof(Nr), true);
        offset += sizeof(Nr);
        BytesTool::writeBytes(Length, buffer, offset, sizeof(Length), true);
        offset += sizeof(Length);
        BytesTool::writeBytes(Index, buffer, offset, sizeof(Index), true);
        offset += sizeof(Index);

        return 0x0;
    }
public:
    const int Size;//类的const变量在构造函数初始化列表中完成初始化
    ushort Nr;
    int Length;
    int Index;
};

// Defined in SCP.
byte[] SCPSection0::_Reserved = {(byte)'S', (byte)'C', (byte)'P', (byte)'E', (byte)'C', (byte)'G'};
ushort SCPSection0::_SectionID = 0;
int SCPSection0::_NrMandatory = 12;
/// <summary>
/// Class contains section 0 (Pointer section).
/// </summary>

SCPSection0::SCPSection0()
{
    // Part of the stored Data Structure.
    SCPPointer[] _MandatoryPointers = new SCPPointer[_NrMandatory];
    SCPPointer[] _OptionalPointers = null;
}

int SCPSection0::_Write(byte[] buffer, int offset)
{
    for (int loper=0;loper < _NrMandatory;loper++)
    {
        int err = _MandatoryPointers[loper].Write(buffer, offset);
        if (err != 0)
        {
            return err;
        }
        offset += SCPPointer.Size;
    }
    if (_OptionalPointers != null)
    {
        for (int loper=0;loper < _OptionalPointers.Length;loper++)
        {
            int err = _OptionalPointers[loper].Write(buffer, offset);
            if (err != 0)
            {
                return err << 1;
            }
            offset += SCPPointer.Size;
        }
    }
    return 0x0;
}

void SCPSection0::_Empty()
{
    BytesTool::copy(Reserved, 0, _Reserved, 0, 6);
    if ((this._MandatoryPointers != null)
            &&	(_MandatoryPointers.Length == _NrMandatory))
    {
        for (int loper=0;loper < _NrMandatory;loper++)
        {
            _MandatoryPointers[loper] = null;
        }
    }
    else
    {
        _MandatoryPointers = new SCPPointer[_NrMandatory];
    }
    _OptionalPointers = null;
}

ushort SCPSection0::getSectionID()
{
    return _SectionID;
}

int SCPSection0::_getLength()
{
    int sum = SCPPointer.Size * getNrPointers();
    return ((sum % 2) == 0 ? sum : sum + 1);
}

bool SCPSection0::Works()
{
    if ((_MandatoryPointers != null)
            &&  (_MandatoryPointers.Length == _NrMandatory))
    {
        for (int loper=0;loper < _NrMandatory;loper++)
        {
            if (_MandatoryPointers[loper] == null)
            {
                return false;
            }
        }
        if (_OptionalPointers != null)
        {
            for (int loper=0;loper < _OptionalPointers.Length;loper++)
            {
                if (_OptionalPointers[loper] == null)
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}

/// <summary>
/// Function to get number of pointers contained in pointer section.
/// </summary>
/// <returns>number of pointers</returns>
int SCPSection0::getNrPointers()
{
    return _NrMandatory + (_OptionalPointers != null ? _OptionalPointers.Length : 0);
}

/// <summary>
/// Function to set number of pointers contained in pointer section.
/// </summary>
/// <param name="nr">number of pointers</param>
void SCPSection0::setNrPointers(int nr)
{
    int current = _NrMandatory + (_OptionalPointers != null ? _OptionalPointers.Length : 0);
    if ((nr != current)
            &&  (nr >= _NrMandatory))
    {
        _MandatoryPointers = new SCPPointer[_NrMandatory];
        if (nr > _NrMandatory)
        {
            _OptionalPointers = new SCPPointer[nr - _NrMandatory];
        }
    }
}

/// <summary>
/// Function to get section id of a pointer.
/// </summary>
/// <param name="nr">pointer number to get section id from</param>
/// <returns>id of section</returns>
ushort SCPSection0::getSectionID(int nr)
{
    if ((nr >= 0)
            &&  (nr < getNrPointers()))
    {
        if (nr < _NrMandatory)
        {
            return _MandatoryPointers[nr].Nr;
        }
        return _OptionalPointers[nr - _NrMandatory].Nr;
    }
    return 0;
}

/// <summary>
/// Function to set section id of a pointer.
/// </summary>
/// <param name="nr">number of pointer to set</param>
/// <param name="ID">id of section</param>
void SCPSection0::setSectionID(int nr, ushort ID)
{
    nr -= _NrMandatory;
    if ((_OptionalPointers != null)
            &&	(nr >= 0)
            &&  (nr < _OptionalPointers.Length)
            &&  (_OptionalPointers[nr] != null))
    {
        _OptionalPointers[nr].Nr = ID;
    }
}

/// <summary>
/// Function to get index of a pointer.
/// </summary>
/// <param name="nr">number of pointer to get from</param>
/// <returns>0 on success</returns>
int SCPSection0::getIndex(int nr)
{
    if ((nr >= 0)
            &&  (nr < getNrPointers()))
    {
        if (nr < _NrMandatory)
        {
            return _MandatoryPointers[nr].Index;
        }
        return _OptionalPointers[nr - _NrMandatory].Index;
    }
    return 0;
}

/// <summary>
/// Function to set index of a pointer.
/// </summary>
/// <param name="nr">number of pointer to set</param>
/// <param name="index">index to set pointer to</param>
void SCPSection0::setIndex(int nr, int index)
{
    if ((nr >= 0)
            &&  (nr < getNrPointers()))
    {
        if (nr < _NrMandatory)
        {
            _MandatoryPointers[nr].Index = index;
        }
        else
        {
            _OptionalPointers[nr - _NrMandatory].Index = index;
        }
    }
}

/// <summary>
/// Function to get length of section from pointer.
/// </summary>
/// <param name="nr">number of pointer to get from</param>
/// <returns>length of section</returns>
int SCPSection0::getLength(int nr)
{
    if ((nr >= 0)
            &&  (nr < getNrPointers()))
    {
        if (nr < _NrMandatory)
        {
            return _MandatoryPointers[nr].Length;
        }
        return _OptionalPointers[nr - _NrMandatory].Length;
    }
    return 0;
}

/// <summary>
/// Function to get length of section from pointer.
/// </summary>
/// <param name="nr">number of pointer to set</param>
/// <param name="length">length of section</param>
void SCPSection0::setLength(int nr, int length)
{
    if ((nr >= 0)
            &&  (nr < getNrPointers()))
    {
        if (nr < _NrMandatory)
        {
            _MandatoryPointers[nr].Length = length;
        }
        else
        {
            _OptionalPointers[nr - _NrMandatory].Length = length;
        }
    }
}

/// <summary>
/// Function to get all values of a pointer.
/// </summary>
/// <param name="nr">number of pointer to read from</param>
/// <param name="id">id of section</param>
/// <param name="length">length of section</param>
/// <param name="index">index of section</param>
void SCPSection0::getPointer(int nr,ushort& id, int& length,int& index)
{
    id = 0;
	length = 0;
	index =0;
    if ((nr >= 0)
            &&  (_MandatoryPointers != null)
            &&	(_MandatoryPointers.Length == _NrMandatory))
    {
        if (nr < _NrMandatory)
        {
            if (_MandatoryPointers[nr] != null)
            {
                id = _MandatoryPointers[nr].Nr;
                length = _MandatoryPointers[nr].Length;
                index = _MandatoryPointers[nr].Index;
            }
        }
        else if ((_OptionalPointers != null)
                 &&	 ((nr - _NrMandatory) < _OptionalPointers.Length))
        {
            nr -= _NrMandatory;
            if (_OptionalPointers[nr] == null)
            {
                id = _OptionalPointers[nr].Nr;
                length = _OptionalPointers[nr].Length;
                index = _OptionalPointers[nr].Index;
            }
        }
    }
}

/// <summary>
/// Function to set all values of a pointer
/// </summary>
/// <param name="nr">number of pointer to set</param>
/// <param name="id">id of section</param>
/// <param name="length">length of section</param>
/// <param name="index">index of section</param>
void SCPSection0::setPointer(int nr, ushort id, int length, int index)
{
    if (nr >= 0)
    {
        if (nr < _NrMandatory)
        {
            if (_MandatoryPointers[nr] == null)
            {
                _MandatoryPointers[nr] = new SCPPointer();
            }
            _MandatoryPointers[nr].Nr = id;
            _MandatoryPointers[nr].Length = length;
            _MandatoryPointers[nr].Index = index;
        }
        else if ((_OptionalPointers != null)
                 &&	 ((nr - _NrMandatory) < _OptionalPointers.Length))
        {
            nr -= _NrMandatory;
            if (_OptionalPointers[nr] == null)
            {
                _OptionalPointers[nr] = new SCPPointer();
            }
            _OptionalPointers[nr].Nr = id;
            _OptionalPointers[nr].Length = length;
            _OptionalPointers[nr].Index = index;
        }
    }
}

}
