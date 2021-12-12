#include "SCPSection0.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class containing a SCP pointer.
/// </summary>
class SCPSection0::SCPPointer
{
public:
    SCPPointer()
    {
    }
    /// <summary>
    /// Function to write SCP pointer.
    /// </summary>
    /// <param name="buffer">byte array to write section into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength,int offset)
    {
        if ((offset + Size) > bufferLength)
        {
        	SCP_PE("out of range,offset: %d,Size: %d,bufferLength: %d\n",offset, Size, bufferLength);
            return 0x1;
        }

        BytesTool::writeBytes(Nr, buffer, bufferLength,offset, sizeof(Nr), true);
        offset += sizeof(Nr);
        BytesTool::writeBytes(Length, buffer, bufferLength,offset, sizeof(Length), true);
        offset += sizeof(Length);
        BytesTool::writeBytes(Index, buffer, bufferLength,offset, sizeof(Index), true);
        offset += sizeof(Index);
		SCP_PD("write over,offset: %d,Size: %d,bufferLength: %d\n",offset, Size, bufferLength);
        return 0x0;
    }
public:
    static int Size;//类的const变量在构造函数初始化列表中完成初始化
    ushort Nr;
    int Length;
    int Index;
};

int SCPSection0::SCPPointer::Size = 10;

// Defined in SCP.
uchar SCPSection0::_Reserved[6] = {'S', 'C', 'P', 'E', 'C', 'G'};
ushort SCPSection0::_SectionID = 0;
/// <summary>
/// Class contains section 0 (Pointer section).
/// </summary>

SCPSection0::SCPSection0()
{
    // Part of the stored Data Structure.
    _MandatoryPointers.reserve(kNrMandatory);
}

int SCPSection0::_Write(uchar* buffer, int bufferLength, int offset)
{
    for (int loper=0;loper < kNrMandatory;loper++)
    {
        int err = _MandatoryPointers[loper].Write(buffer, bufferLength, offset);
        if (err != 0)
        {
        	SCP_PE("Write failed,_MandatoryPointers: %d\n",loper);
            return err;
        }
        offset += SCPPointer::Size;
    }
    return 0x0;
}

void SCPSection0::_Empty()
{
    BytesTool::copy(Reserved, SCPSection::kReservedLength,0, _Reserved, 6,0, 6);
    _MandatoryPointers.clear();
}

ushort SCPSection0::getSectionID()
{
    return _SectionID;
}

int SCPSection0::_getLength()
{
    int sum = SCPPointer::Size * getNrPointers();
    return ((sum % 2) == 0 ? sum : sum + 1);
}

bool SCPSection0::Works()
{
    if (_MandatoryPointers.size() > 0)
    {
        return true;
    }
	SCP_PW("not Works\n");
    return false;
}

/// <summary>
/// Function to get number of pointers contained in pointer section.
/// </summary>
/// <returns>number of pointers</returns>
int SCPSection0::getNrPointers()
{
    return kNrMandatory;
}

/// <summary>
/// Function to set number of pointers contained in pointer section.
/// </summary>
/// <param name="nr">number of pointers</param>
void SCPSection0::setNrPointers(int nr)
{
    int current = kNrMandatory;
    if ((nr != current)
            &&  (nr >= kNrMandatory))
    {
		_MandatoryPointers.reserve(kNrMandatory);
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
        if (nr < kNrMandatory)
        {
            return _MandatoryPointers[nr].Nr;
        }
    }
    return 0;
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
        if (nr < kNrMandatory)
        {
            return _MandatoryPointers[nr].Index;
        }
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
    if ((nr >= 0)  && (nr < getNrPointers()))
    {
        if (nr < kNrMandatory)
        {
            _MandatoryPointers[nr].Index = index;
			SCP_PD("nr: %d,index: %d\n",nr,index);
			return;
        }
    }
	SCP_PW("out of range,nr: %d\n",nr);
}

/// <summary>
/// Function to get length of section from pointer.
/// </summary>
/// <param name="nr">number of pointer to get from</param>
/// <returns>length of section</returns>
int SCPSection0::getLength(int nr)
{
    if ((nr >= 0)  && (nr < getNrPointers()))
    {
        if (nr < kNrMandatory)
        {
			SCP_PD("nr: %d,length: %d\n",nr,_MandatoryPointers[nr].Length);
            return _MandatoryPointers[nr].Length;
        }
    }
	SCP_PW("out of range,nr: %d\n",nr);
    return 0;
}

/// <summary>
/// Function to get length of section from pointer.
/// </summary>
/// <param name="nr">number of pointer to set</param>
/// <param name="length">length of section</param>
void SCPSection0::setLength(int nr, int length)
{
    if ((nr >= 0)  && (nr < getNrPointers()))
    {
        if (nr < kNrMandatory)
        {
            _MandatoryPointers[nr].Length = length;
			SCP_PD("nr: %d,length: %d\n",nr,length);
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
    if ((0 <= nr) && (nr < _MandatoryPointers.size()))
    {
        id = _MandatoryPointers[nr].Nr;
        length = _MandatoryPointers[nr].Length;
        index = _MandatoryPointers[nr].Index;
		return;
    }
	SCP_PW("out of range,nr: %d\n",nr);
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
    if ((0 <= nr) && (nr < _MandatoryPointers.size()))
    {
        _MandatoryPointers[nr].Nr = id;
        _MandatoryPointers[nr].Length = length;
        _MandatoryPointers[nr].Index = index;
		return;
    }
	SCP_PW("out of range,nr: %d\n",nr);
}
}
}
