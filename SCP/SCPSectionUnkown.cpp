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

SCPSectionUnknown::SCPSectionUnknown(ushort sectionId)
{
    this->SectionID = sectionId;
}

int SCPSectionUnknown::_Write(uchar* buffer, int bufferLength, int offset)
{
    offset += BytesTool::copy(buffer, bufferLength, offset, _Data, _DataLength, 0, _DataLength);
    return 0x00;
}

void SCPSectionUnknown::_Empty()
{
    delete []_Data;
}

int SCPSectionUnknown::_getLength()
{
    if (Works()) {
        return _DataLength;
    }

    return 0;
}

ushort SCPSectionUnknown::getSectionID()
{
    return SectionID;
}

bool SCPSectionUnknown::Works()
{
    if (_Data != null) {
        return true;
    }

    return false;
}

void SCPSectionUnknown::setInternalData(uchar* InternalData, int InternalDataLength)
{
    if ((InternalData != null) && (InternalDataLength > 0)) {
        if (_Data != null) {
            delete [] _Data;
        }

        memcpy(_Data, InternalData, InternalDataLength);
    }
}
}
}
