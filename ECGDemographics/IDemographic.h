#ifndef _ZQ_IDEMOGRAPHIC_CS_
#define _ZQ_IDEMOGRAPHIC_CS_
    
namespace ECGConversion.ECGDemographics
{
	/// <summary>
	/// Interface for manupalation of demograpics information.
	/// </summary>
	class IDemographic
	{
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
		virtual void setPatientBirthDate(Date PatientBirthDate) = 0;
		virtual int setPatientHeight(ushort val, HeightDefinition def) = 0;
		virtual int setPatientWeight(ushort val, WeightDefinition def) = 0;
		virtual void setGender(Sex Gender) = 0;
		virtual void setPatientRace(Race PatientRace) = 0;
		virtual void setAcqMachineID(AcquiringDeviceID AcqMachineID) = 0;
		virtual void setAnalyzingMachineID(AcquiringDeviceID AnalyzingMachineID) = 0;
		virtual void setTimeAcquisition(DateTime TimeAcquisition) = 0;
		virtual void setBaselineFilter(ushort BaselineFilter) = 0;
		virtual void setLowpassFilter(ushort LowpassFilter) = 0;
		virtual void setFilterBitmap(byte FilterBitmap) = 0;
		virtual void setFreeTextFields(string[] FreeTextFields) = 0;
		virtual void setSequenceNr(string SequenceNr) = 0;
		virtual void setAcqInstitution(string AcqInstitution) = 0;
		virtual void setAnalyzingInstitution(string AnalyzingInstitution) = 0;
		virtual void setAcqDepartment(string AcqDepartment) = 0;
		virtual void setAnalyzingDepartment(string AnalyzingDepartment) = 0;
		virtual void setReferringPhysician(string ReferringPhysician) = 0;
		virtual void setOverreadingPhysician(string OverreadingPhysician) = 0;
		virtual void setTechnicianDescription(string TechnicianDescription) = 0;
		virtual void setSystolicBloodPressure(ushort SystolicBloodPressure) = 0;
		virtual void setDiastolicBloodPressure(ushort DiastolicBloodPressure) = 0;
		virtual void setDrugs(Drug[] Drugs) = 0;
		virtual void setReferralIndication(string[] ReferralIndication) = 0;
		virtual void setRoomDescription(string RoomDescription) = 0;
		virtual void setStatCode(byte StatCode) = 0;
	};
}


#endif  /*#ifndef _ZQ_IDEMOGRAPHIC_CS_*/
