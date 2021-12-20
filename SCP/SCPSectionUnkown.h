#ifndef _SCPSECTIONUNKOWN_H_
#define _SCPSECTIONUNKOWN_H_
#include "SCPSection.h"

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// SCP Unknown Section. this class is for reading and writing SCP formats without having to implement the sections that aren't needed at all.
/// </summary>
class SCPSectionUnknown : public SCPSection
{
public:
    SCPSectionUnknown();
    SCPSectionUnknown(ushort sectionId);
    ushort getSectionID();
    bool Works();
    /// <summary>
    /// Get and Set the internal data of this
    /// </summary>
    void setInternalData(uchar* InternalData, int InternalDataLength);
protected:
    int _Write(uchar* buffer, int bufferLength, int offset);
    void _Empty();
    int _getLength();
    // Part of the stored Data Structure.
private:
    uchar* _Data;
    int _DataLength;
};
}
}
#endif  /*#ifndef _SCPSECTIONUNKOWN_H_*/
