#include "AcquiringDeviceID.h"

namespace ECGConversion.ECGDemographics
{
/// <summary>
/// Device information that can be imported and exported in both SCP and UNIPRO.
ushort AcquiringDeviceID::_ModelDescriptionLen = 6;

AcquiringDeviceID::AcquiringDeviceID()
{
	InstitutionNr = 0;
	DepartmentNr = 0;
	DeviceID = 0;
	DeviceType = 0;
	ManufactorID = (byte) DeviceManufactor.Unknown;
	DeviceCapabilities = 0; // Is defined in SCP Section1 tag 14 byte 18.
	ACFrequencyEnvironment = 0; // Is defined in SCP Section1 tag 14 byte 19.
	_ModelDescription = new byte[_ModelDescriptionLen];
}

AcquiringDeviceID::AcquiringDeviceID(bool bNoDeviceId)
{
    if (bNoDeviceId)
    {
        InstitutionNr = 0;
        DepartmentNr = 11;
        DeviceID = 51;
        DeviceType = (byte) ECGConversion.ECGDemographics.DeviceType.System;
        ManufactorID = (byte) DeviceManufactor.Unknown;
        DeviceCapabilities = 0x8;
        ACFrequencyEnvironment = 1;
        Communication_IO_Tools.BytesTool.writeString("MCONV", ModelDescription, 0, ModelDescription.Length);
    }
    _ModelDescription = new byte[_ModelDescriptionLen];
}

byte[] AcquiringDeviceID::getModelDescription()
{
    return _ModelDescription;
}

}
