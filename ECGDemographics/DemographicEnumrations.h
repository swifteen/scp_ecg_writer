#ifndef _DEMOGRAPHICENUMRATIONS_H_
#define _DEMOGRAPHICENUMRATIONS_H_

namespace ECGConversion
{
namespace ECGDemographics
{

/// <summary>
/// An Enumration to determine the sex.
/// </summary>
enum Sex
{
    kSexUnspecified = 0,
    kSexMale,
    kSexFemale,
    kSexNullSex = 0xff
};
/// <summary>
/// An Enumration to determine the race.
/// </summary>
enum Race
{
    kRaceUnspecified = 0,
    kRaceCaucasian,
    kRaceBlack,
    kRaceOriental,
    kRaceNull = 0xff
};
/// <summary>
/// An Enumration to determine the age definition.
/// </summary>
enum AgeDefinition
{
    kAgeUnspecified = 0,
    kAgeYears,
    kAgeMonths,
    kAgeWeeks,
    kAgeDays,
    kAgeHours
};
/// <summary>
/// An Enumration to determine the height definition.
/// </summary>
enum HeightDefinition
{
    kHeightUnspecified = 0,
    kHeightCentimeters,
    kHeightInches,
    kHeightMillimeters
};
/// <summary>
/// An Enumration to determine the weight definition.
/// </summary>
enum WeightDefinition
{
    kWeightUnspecified = 0,
    kWeightKilogram,
    kWeightGram,
    kWeightPound,
    kWeightOunce
};
/// <summary>
/// An Enumration to determine the device type.
/// </summary>
enum DeviceType
{
    kDeviceTypeCart = 0,
    kDeviceTypeSystem
};
/// <summary>
/// An Enumration to determine the device manufactor.
/// </summary>
enum DeviceManufactor
{
    kDeviceManufactorUnknown = 0,
    kDeviceManufactorBurdick,
    kDeviceManufactorCambridge,
    kDeviceManufactorCompumed,
    kDeviceManufactorDatamed,
    kDeviceManufactorFukuda,
    kDeviceManufactorHewlettPackard,
    kDeviceManufactorMarquetteElectronics,
    kDeviceManufactorMortaraInstruments,
    kDeviceManufactorNihonKohden,
    kDeviceManufactorOkin,
    kDeviceManufactorQuintin,
    kDeviceManufactorSiemens,
    kDeviceManufactorSpaceLabs,
    kDeviceManufactorTelemed,
    kDeviceManufactorHellige,
    kDeviceManufactorESAOTE,
    kDeviceManufactorSchiller,
    kDeviceManufactorPickerSchwarzer,
    kDeviceManufactorElettronicTrentina,
    kDeviceManufactorZwonitz,
    kDeviceManufactorOther = 100
};
/// <summary>
/// An Enumration for Electrode Configurations for 12-lead ECG
/// </summary>
enum ElectrodeConfigCodeTwelveLead
{
    kElectrodeTwelveLeadUnspecified = 0,
    kElectrodeTwelveLeadStandardTwelveLead,
    kElectrodeTwelveLeadMasonLikarAndIndividual,
    kElectrodeTwelveLeadMasonLikarAndPadded,
    kElectrodeTwelveLeadAllLeadPadded,
    kElectrodeTwelveLeadTwelveLeadDerivedXYZ,
    kElectrodeTwelveLeadTwelveLeadDerivedNonStandard
};
/// <summary>
/// An Enumration for Electrode Configurations for XYZ ECG
/// </summary>
enum ElectrodeConfigCodeXYZ
{
    kElectrodeXYZUnspecified = 0,
    kElectrodeXYZFrank,
    kElectrodeXYZMcFeeParungao,
    kElectrodeXYZCube,
    kElectrodeXYZBipolarUncorrected,
    kElectrodeXYZPseudoOrthogonal,
    kElectrodeXYZXYZDerivedTwelveLead
};
}
}
#endif
