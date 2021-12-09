#ifndef _SCPSECTION1_H_
#define _SCPSECTION1_H_
#include "SCPSection.h"
#include "IDemographic.h"    
#include"Drug.h"

using namespace ECGConversion.ECGDemographics;

namespace ECGConversion.SCP
{

/// <summary>
/// Class contains section 1 (Header Information).
/// </summary>
class SCPSection1 : SCPSection, IDemographic
{
public:
    enum ProtocolCompatibility
    {CatI = 0x90, CatII = 0xa0, CatIII = 0xb0, CatIV = 0xc0};
private:
    class SCPHeaderField;
public:
    SCPSection1();
    ushort getSectionID();
    bool Works();
    /// <summary>
    /// Function to initialize a section 1. Only needed when not reading from buffer.
    /// </summary>
    void Init();
    /// <summary>
    /// Function to insert a field into section.
    /// </summary>
    /// <param name="field">field to insert</param>
    /// <returns>0 on success</returns>
    int Insert(SCPHeaderField field);
    /// <summary>
    /// Function to remove a certain field from section.
    /// </summary>
    /// <param name="tag">tag of field.</param>
    /// <returns>0 on success</returns>
    int Remove(byte tag);
    /// <summary>
    /// Function to resize the space for header fields.
    /// </summary>
    void Resize();
    /// <summary>
    /// Function to get a field from this section
    /// </summary>
    /// <param name="tag">tag to search for</param>
    /// <returns></returns>
    SCPHeaderField GetField(byte tag);
    /// <summary>
    /// Get encoding for text from language support code.
    /// </summary>
    /// <returns>used encoding</returns>
    System.Text.Encoding getLanguageSupportCode();
    /// <summary>
    /// Get encoding for text from language support code.
    /// </summary>
    /// <param name="enc">used encoding</param>
    /// <returns>0 if successfull</returns>
    int getLanguageSupportCode(out System.Text.Encoding enc);
    /// <summary>
    /// Set language support code based on encoding.
    /// </summary>
    /// <param name="enc">encoding to set lsc with.</param>
    /// <returns>0 if successfull</returns>
    int setLanguageSupportCode(System.Text.Encoding enc);
    /// <summary>
    /// Function to get Protocol Compatability Level.
    /// </summary>
    /// <param name="pc">Protocol Compatability Level</param>
    /// <returns>0 on succes</returns>
    int getProtocolCompatibilityLevel(out ProtocolCompatibility pc);
    /// <summary>
    /// Function to set Protocol Compatability Level.
    /// </summary>
    /// <param name="pc">Protocol Compatability Level</param>
    /// <returns>0 on succes</returns>
    int setProtocolCompatibilityLevel(ProtocolCompatibility pc);
    /// <summary>
    /// Function to set a text from a cetain tag.
    /// </summary>
    /// <param name="tag">id of tag</param>
    /// <param name="text">a string</param>
    /// <returns>0 on success</returns>
    int setText(byte tag, const string& text);

    // Getting Demographics information
    void setLastName(const string& value);
    void setFirstName(const string& value);
    void setPatientID(const string& value);
    void setSecondLastName(const string& value);
    int setPatientAge(ushort val, AgeDefinition def);
    void setPatientBirthDate(Date PatientBirthDate);
    int setPatientHeight(ushort val, HeightDefinition def);
    int setPatientWeight(ushort val, WeightDefinition def);
    void setGender(Sex Gender);
    void setPatientRace(Race PatientRace);
    void setAcqMachineID(AcquiringDeviceID AcqMachineID);
    void setAnalyzingMachineID(AcquiringDeviceID AnalyzingMachineID);
    void setTimeAcquisition(DateTime TimeAcquisition);
    void setBaselineFilter(ushort BaselineFilter);
    void setLowpassFilter(ushort LowpassFilter);
    void setFilterBitmap(byte FilterBitmap);
    void setFreeTextFields(string[] FreeTextFields);
    void setSequenceNr(string SequenceNr);
    void setAcqInstitution(string AcqInstitution);
    void setAnalyzingInstitution(string AnalyzingInstitution);
    void setAcqDepartment(string AcqDepartment);
    void setAnalyzingDepartment(string AnalyzingDepartment);
    void setReferringPhysician(string ReferringPhysician);
    void setOverreadingPhysician(string OverreadingPhysician);
    void setTechnicianDescription(string TechnicianDescription);
    void setSystolicBloodPressure(ushort SystolicBloodPressure);
    void setDiastolicBloodPressure(ushort DiastolicBloodPressure);
    void setDrugs(Drug[] Drugs);
    void setReferralIndication(string[] ReferralIndication);
    void setRoomDescription(string RoomDescription);
    void setStatCode(byte StatCode);
protected:
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    /// <summary>
    /// Function to search for a field with a certain tag.
    /// </summary>
    /// <param name="tag">tag to search for</param>
    /// <returns>position of this field</returns>
    int _SearchField(byte tag);
    /// <summary>
    /// Function to find position to insert a field with a certain tag.
    /// </summary>
    /// <param name="tag">tag to search on.</param>
    /// <returns>position to insert</returns>
    int _InsertSearch(byte tag);
    /// <summary>
    /// Function to check wheter the used fields are indeed sorted.
    /// </summary>
    bool CheckInstances();
    /// <summary>
    /// Function to check for exception case.
    /// </summary>
    /// <param name="condition">condition</param>
    /// <param name="tag">value of tag</param>
    /// <returns>is exception then true</returns>
    static bool isException(byte[] condition, byte tag);
private:
    // Fields that must be made empty for anonymous. (must be sorted from low to high)
    static byte[] _AnonymousFields;
    // Defined in SCP.
    static byte[] _MustBePresent; // defined in paragraph 5.4.3.1 of SCP
    static byte[] _MultipleInstanceFields; // Must be sorted
    static ushort _MaximumFieldLength;
    static byte[] _MaximumLengthExceptions; // Must be sorted
    static ushort _ExceptionsMaximumLength; // should be 80, but some scp file doen't use this maximum. apparantly 128 wasn't enough as well
    static byte _ManufactorField;
    static ushort _SectionID;
    static byte _DemographicTerminator;

    // ResizeSpeed for the array to store the Fields.
    static int _ResizeSpeed;

    // Part of the stored Data Structure.
    int _NrFields;
    SCPHeaderField[] _Fields;
};
}
#endif  /*#ifndef _SCPSECTION1_H_*/
