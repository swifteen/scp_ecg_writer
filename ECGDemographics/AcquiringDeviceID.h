#ifndef _ZQ_ACQUIRINGDEVICEID_H_
#define _ZQ_ACQUIRINGDEVICEID_H_

namespace ECGConversion.ECGDemographics
{
/// <summary>
/// Device information that can be imported and exported in both SCP and UNIPRO.
/// </summary>
class AcquiringDeviceID
{
public:
    AcquiringDeviceID();
    AcquiringDeviceID(bool bNoDeviceId);
    byte[] getModelDescription();
public:
    ushort InstitutionNr;
    ushort DepartmentNr;
    ushort DeviceID;
    byte DeviceType;
    byte ManufactorID;
    byte DeviceCapabilities ; // Is defined in SCP Section1 tag 14 byte 18.
    byte ACFrequencyEnvironment; // Is defined in SCP Section1 tag 14 byte 19.
private:
    static int _ModelDescriptionLen ;
    byte[] _ModelDescription ;
};
}
