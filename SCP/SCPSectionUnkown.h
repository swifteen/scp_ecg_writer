#ifndef _SCPSECTIONUNKOWN_H_
#define _SCPSECTIONUNKOWN_H_
#include "SCPSection.h"

namespace ECGConversion.SCP
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
    void setInternalData(byte[] InternalData);
protected:
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
    // Part of the stored Data Structure.
private:
    byte[] _Data;
};
}

#endif  /*#ifndef _SCPSECTIONUNKOWN_H_*/
