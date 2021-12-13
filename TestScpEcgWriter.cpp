#include "SCPFormat.h"
using namespace ECGConversion::SCP;
using namespace ECGConversion::ECGSignals;
using namespace ECGConversion::ECGDemographics;
using namespace ECGConversion::ECGDiagnostic;	
using namespace ECGConversion::ECGGlobalMeasurements;
using namespace ECGConversion::ECGLeadMeasurements;

static int DemographicCopy(IDemographic* dst)
{
	// Check for correct input.
	if (dst == null)
	{
		return 1;
	}

	// Do copy of all Demographics.
	int err = 0;
	dst->setLastName("src.LastName");
	dst->setFirstName("src.FirstName");
	dst->setPatientID("src.PatientID");
	dst->setSecondLastName("src.SecondLastName");

#if 0
	dst->setPrefixName("src.PrefixName");
	if (dst->setPrefixName == null)
		err |= (0x1 << 29);
	
	dst->setSuffixName()src.SuffixName;
	if (dst->setSuffixName == null)
		err |= (0x1 << 30);
#endif
	
	ushort val = 20;
	AgeDefinition age = kAgeYears;

	if (dst->setPatientAge(val, age) != 0)
	{
		err |= (0x1 << 4);
	}

	Date date = Date(1988, 1, 18);
	dst->setPatientBirthDate(date);

	HeightDefinition height = kHeightCentimeters;
	val = 170;
	if (dst->setPatientHeight(val, height) != 0)
	{
		err |= (0x1 << 6);
	}

	WeightDefinition weight = kWeightKilogram;	
	val = 50;
	if (dst->setPatientWeight(val, weight) != 0)
	{
		err |= (0x1 << 7);
	}

	dst->setGender(kSexMale);	
	dst->setPatientRace(kRaceCaucasian);

	AcquiringDeviceID AcqMachineID;	
    AcqMachineID.InstitutionNr = 1;
    AcqMachineID.DepartmentNr = 1;
    AcqMachineID.DeviceID = 1244;
    AcqMachineID.DeviceType = 1;
    AcqMachineID.ManufactorID = 1;
    AcqMachineID.DeviceCapabilities = 1; // Is defined in SCP Section1 tag 14 uchar 18.
    AcqMachineID.ACFrequencyEnvironment = 1; // Is defined in SCP Section1 tag 14 uchar 19.
    uchar ModelDescription[6];
	dst->setAcqMachineID(AcqMachineID);

	dst->setAnalyzingMachineID(AcqMachineID);
	
	DateTime dateTime;
	dateTime.Year = 2021; 
	dateTime.Month = 12; 
	dateTime.Day = 12; 
	dateTime.Hour = 20; 
	dateTime.Minute = 0; 
	dateTime.Second = 0; 
	dst->setTimeAcquisition(dateTime);

	dst->setBaselineFilter(50);
	dst->setLowpassFilter(10);	
	dst->setFilterBitmap(10);

	std::vector<string> FreeTextFields;
	FreeTextFields.clear();
	string freeText = "test freeText";
	FreeTextFields.push_back(freeText);
	dst->setFreeTextFields(FreeTextFields);
	dst->setSequenceNr("src.SequenceNr");
	dst->setAcqInstitution("src.AcqInstitution");
	dst->setAnalyzingInstitution("src.AnalyzingInstitution");
	dst->setAcqDepartment("src.AcqDepartment");
	dst->setAnalyzingDepartment("src.AnalyzingDepartment");
	dst->setReferringPhysician("src.ReferringPhysician");
	dst->setOverreadingPhysician("src.OverreadingPhysician");
	dst->setTechnicianDescription("src.TechnicianDescription");

	dst->setSystolicBloodPressure(50);
	dst->setDiastolicBloodPressure(60);
	
	std::vector<Drug> Drugs;
	Drugs.clear();
	Drugs.push_back(Drug(1,2,"Drug TextDesciption"));
	dst->setDrugs(Drugs);

	std::vector<string> ReferralIndication;
	ReferralIndication.clear();
	string referralText = "test referralText";
	ReferralIndication.push_back(referralText);
	dst->setReferralIndication(ReferralIndication);

	dst->setRoomDescription("src.RoomDescription");
	dst->setStatCode(212);
	return err;
}

static int SignalCopy(ISignal* dst)
{
	// Check for correct input.
	if (dst == null)
	{
		return 2;
	}

	// Do copy of signals
	Signals signals;
	signals.setNrLeads(12);
	signals.SetAVM(2.5);// AVM in uV
	signals.RhythmSamplesPerSecond = 500;
	std::vector<Signal> leads;
	LeadType leadTypeArray[] = 
	{	
		LeadTypeI, 
		LeadTypeII, 
		LeadTypeV1, 
		LeadTypeV2, 
		LeadTypeV3, 
		LeadTypeV4, 
		LeadTypeV5, 
		LeadTypeV6,
		LeadTypeIII, 
		LeadTypeaVR, 
		LeadTypeaVL, 
		LeadTypeaVF,
	};
	for (int i = 0; i < 12; i++)
	{
		Signal leadSignal;
		leadSignal.Type = leadTypeArray[i];
		leadSignal.RhythmStart = 0;
		leadSignal.RhythmEnd = 5000;		
		leadSignal.Rhythm = new short[5000];
		for (int k = 0; k < 5000; k++)
		{
			leadSignal.Rhythm[k] = i;
		}
		leadSignal.RhythmLength = 5000;
		leads.push_back(leadSignal);
	}
	
	signals.setLeads(leads);
	if (dst->setSignals(signals) != 0)
	{
		//TODO free leadSignal.Rhythm
		return 8;
	}
	//TODO free leadSignal.Rhythm
	return 0;
}

static int DiagnosticCopy(IDiagnostic* dst)
{
	// Check for correct input.
	if (dst == null)
	{
		return 2;
	}

	// Do copy of diagnostic
	Statements stat;
	stat.confirmed = false;
	stat.interpreted = false;
	stat.time.Year = 2021; 
	stat.time.Month = 12; 
	stat.time.Day = 12; 
	stat.time.Hour = 20; 
	stat.time.Minute = 0; 
	stat.time.Second = 0; 
	stat.statement.push_back("test text");
	if (dst->setDiagnosticStatements(stat) != 0)
	{
		return 8;
	}
	return 0;
}

static int GlobalMeasurementCopy(IGlobalMeasurement* dst)
{
	// Check for correct input.
	if (dst == null)
	{
		return 2;
	}

	// Do copy of measurements
	GlobalMeasurements mes;
	mes.setVentRate(80);
	mes.setPdur(50);
	mes.setPRint(60);
	mes.setQRSdur(70);
	mes.setQTdur(80);
	mes.setQTc(90);
	mes.setQTcType(GlobalMeasurement::QTcCalcTypeBazett);
	if (dst->setGlobalMeasurements(mes) != 0)
	{
		return 8;
	}
	return 0;
}


static int LeadMeasurementCopy(ILeadMeasurement* dst)
{
	// Check for correct input.
	if (dst == null)
	{
		return 2;
	}
	//average template
	LeadMeasurement leadMeas1;
	leadMeas1.setMeasurement(MeasurementTypePdur,51);
	leadMeas1.setMeasurement(MeasurementTypeQRSdur,51);
	
	LeadMeasurement leadMeas2;
	leadMeas2.setMeasurement(MeasurementTypePdur,52);
	leadMeas2.setMeasurement(MeasurementTypeQRSdur,52);
	// Do copy of measurements
	LeadMeasurements mes(12);
	mes.Measurements[0] = leadMeas1;
	mes.Measurements[1] = leadMeas2;
	mes.Measurements[2] = leadMeas1;
	mes.Measurements[3] = leadMeas2;
	mes.Measurements[4] = leadMeas1;
	mes.Measurements[5] = leadMeas2;
	mes.Measurements[6] = leadMeas1;
	mes.Measurements[7] = leadMeas2;
	mes.Measurements[8] = leadMeas1;
	mes.Measurements[9] = leadMeas2;
	mes.Measurements[10] = leadMeas1;
	mes.Measurements[11] = leadMeas2;
	if (dst->setLeadMeasurements(mes) != 0)
	{
		return 8;
	}
	return 0;
}


int main()
{
	SCPFormat scp;	
    IDemographic* demdst = scp.getDemographic();
    ISignal* sigdst = scp.getSignals();	
    IDiagnostic* diadst = scp.getDiagnostic();	
    IGlobalMeasurement* mesdst = scp.getGlobalMeasurements();
    ILeadMeasurement* leaddst = scp.getLeadMeasurements();
	if (demdst != null)
	{
		demdst->Init();
		int ret = DemographicCopy(demdst);
		if(ret != 0)
		{
			SCP_PE("DemographicCopy failed,ret[%d]\n",ret);
		}
	}
	if (sigdst != null)
	{
		int ret = SignalCopy(sigdst);
		if(ret != 0)
		{
			SCP_PE("SignalCopy failed,ret[%d]\n",ret);
		}
	}
	if (diadst != null)
	{
		int ret = DiagnosticCopy(diadst);
		if(ret != 0)
		{
			SCP_PE("DiagnosticCopy failed,ret[%d]\n",ret);
		}
	}
	if (mesdst != null)
	{
		int ret = GlobalMeasurementCopy(mesdst);
		if(ret != 0)
		{
			SCP_PE("DemogrGlobalMeasurementCopyaphicCopy failed,ret[%d]\n",ret);
		}
	}
	if (leaddst != null)
	{
		int ret = LeadMeasurementCopy(leaddst);
		if(ret != 0)
		{
			SCP_PE("LeadMeasurementCopy failed,ret[%d]\n",ret);
		}
	}
	scp.setPointers();
	scp.Write("test.scp");	
	return 0;
}
