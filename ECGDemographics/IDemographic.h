#ifndef _IDEMOGRAPHIC_CS_
#define _IDEMOGRAPHIC_CS_
#include "DemographicEnumrations.h"
#include "Date.h"
#include "Drug.h"
#include "AcquiringDeviceID.h"

namespace ECGConversion
{
namespace ECGDemographics
{
	/// <summary>
	/// Interface for manupalation of demograpics information.
	/// </summary>
	class IDemographic
	{
		public:
		/// <summary>
		/// Function to initialize demographics information.
		/// </summary>
		virtual void Init() = 0;
		// Getting Demographics information
		virtual void setLastName(const string& value) = 0;
		virtual void setFirstName(const string& value) = 0;
		virtual void setPatientID(const string& value) = 0;
		virtual void setSecondLastName(const string& value) = 0;
		virtual int setPatientAge(ushort val, AgeDefinition def) = 0;
		virtual void setPatientBirthDate(Date& PatientBirthDate) = 0;
		virtual int setPatientHeight(ushort val, HeightDefinition def) = 0;
		virtual int setPatientWeight(ushort val, WeightDefinition def) = 0;
		virtual void setGender(Sex Gender) = 0;
		virtual void setPatientRace(Race PatientRace) = 0;
		virtual void setAcqMachineID(const AcquiringDeviceID& AcqMachineID) = 0;
		virtual void setAnalyzingMachineID(const AcquiringDeviceID& AnalyzingMachineID) = 0;
		virtual void setTimeAcquisition(const DateTime& time) = 0;
		virtual void setBaselineFilter(ushort BaselineFilter) = 0;
		virtual void setLowpassFilter(ushort LowpassFilter) = 0;
		virtual void setFilterBitmap(uchar FilterBitmap) = 0;
		virtual void setFreeTextFields(const std::vector<string>&      FreeTextFields) = 0;
		virtual void setSequenceNr(const string& SequenceNr) = 0;
		virtual void setAcqInstitution(const string& AcqInstitution) = 0;
		virtual void setAnalyzingInstitution(const string& AnalyzingInstitution) = 0;
		virtual void setAcqDepartment(const string& AcqDepartment) = 0;
		virtual void setAnalyzingDepartment(const string& AnalyzingDepartment) = 0;
		virtual void setReferringPhysician(const string& ReferringPhysician) = 0;
		virtual void setOverreadingPhysician(const string& OverreadingPhysician) = 0;
		virtual void setTechnicianDescription(const string& TechnicianDescription) = 0;
		virtual void setSystolicBloodPressure(ushort SystolicBloodPressure) = 0;
		virtual void setDiastolicBloodPressure(ushort DiastolicBloodPressure) = 0;
		virtual void setDrugs(std::vector<Drug>& Drugs) = 0;
		virtual void setReferralIndication(const std::vector<string>& ReferralIndication) = 0;
		virtual void setRoomDescription(const string& RoomDescription) = 0;
		virtual void setStatCode(uchar StatCode) = 0;
	};
}
}

#endif  /*#ifndef _IDEMOGRAPHIC_CS_*/
