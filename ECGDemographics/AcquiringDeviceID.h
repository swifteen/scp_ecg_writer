#ifndef _ACQUIRINGDEVICEID_H_
#define _ACQUIRINGDEVICEID_H_
#include "ScpGlobal.h"

namespace ECGConversion
{

namespace ECGDemographics
{

/// <summary>
/// Device information that can be imported and exported in both SCP and UNIPRO.
/// </summary>
class AcquiringDeviceID
{
public:
    AcquiringDeviceID();
    AcquiringDeviceID(bool bNoDeviceId);
    uchar* getModelDescription();
public:
    ushort InstitutionNr;
    ushort DepartmentNr;
    ushort DeviceID;
    uchar DeviceType;
    uchar ManufactorID;
    uchar DeviceCapabilities ; // Is defined in SCP Section1 tag 14 uchar 18.
    uchar ACFrequencyEnvironment; // Is defined in SCP Section1 tag 14 uchar 19.
     static int _ModelDescriptionLen ;
private:
   
    uchar *_ModelDescription ;
};
}
}
#endif /*ACQUIRINGDEVICEID_H*/