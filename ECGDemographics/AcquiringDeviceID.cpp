#include "AcquiringDeviceID.h"

namespace ECGConversion
{

namespace ECGDemographics
{

/// <summary>
/// Device information that can be imported and exported in both SCP and UNIPRO.
int AcquiringDeviceID::_ModelDescriptionLen = 6;

AcquiringDeviceID::AcquiringDeviceID()
{
	InstitutionNr = 0;
	DepartmentNr = 0;
	DeviceID = 0;
	DeviceType = 0;
#if 0 // todo	
	ManufactorID = (uchar) DeviceManufactor::Unknown;
#endif
	DeviceCapabilities = 0; // Is defined in SCP Section1 tag 14 byte 18.
	ACFrequencyEnvironment = 0; // Is defined in SCP Section1 tag 14 byte 19.
	_ModelDescription = new uchar[_ModelDescriptionLen];
}

AcquiringDeviceID::AcquiringDeviceID(bool bNoDeviceId)
{
    if (bNoDeviceId)
    {
        InstitutionNr = 0;
        DepartmentNr = 11;
        DeviceID = 51;
#if 0   //todo		
        DeviceType = (uchar) ECGConversion.ECGDemographics.DeviceType.System;
        ManufactorID = (uchar) DeviceManufactor::Unknown;
#endif		
        DeviceCapabilities = 0x8;
        ACFrequencyEnvironment = 1;
#if 0	//todo	
        Communication_IO_Tools.BytesTool.writeString("MCONV", ModelDescription, 0, ModelDescription.Length);
#endif
    }
    _ModelDescription = new uchar[_ModelDescriptionLen];
}

uchar* AcquiringDeviceID::getModelDescription()
{
    return _ModelDescription;
}
}

}
