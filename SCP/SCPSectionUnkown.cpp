#include "SCPSectionUnkown.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// SCP Unknown Section. this class is for reading and writing SCP formats without having to implement the sections that aren't needed at all.
/// </summary>
SCPSectionUnknown::SCPSectionUnknown()
{
    // Part of the stored Data Structure.
    _Data = null;
}

public SCPSectionUnknown::SCPSectionUnknown(ushort sectionId)
{
    this.SectionID = sectionId;
}

int SCPSectionUnknown::_Write(byte[] buffer, int offset)
{
    offset += BytesTool::copy(buffer, offset, _Data, 0, _Data.Length);
    return 0x00;
}

void SCPSectionUnknown::_Empty()
{
    _Data = null;
}

int SCPSectionUnknown::_getLength()
{
    if (Works())
    {
        return _Data.Length;
    }
    return 0;
}

ushort SCPSectionUnknown::getSectionID()
{
    return SectionID;
}

bool SCPSectionUnknown::Works()
{
    if (_Data != null)
    {
        return true;
    }
    return false;
}

void SCPSectionUnknown::setInternalData(byte[] InternalData)
{
    _Data = InternalData;
}
}
}
