#ifndef _ZQ_DEMOGRAPHICENUMRATIONS_H_
#define _ZQ_DEMOGRAPHICENUMRATIONS_H_

namespace ECGConversion.ECGDemographics
{
/// <summary>
/// An Enumration to determine the sex.
/// </summary>
enum Sex
{
    Unspecified = 0,
    Male,
    Female,
    Null = 0xff
};
/// <summary>
/// An Enumration to determine the race.
/// </summary>
enum Race
{
    Unspecified = 0,
    Caucasian,
    Black,
    Oriental,
    Null = 0xff
};
/// <summary>
/// An Enumration to determine the age definition.
/// </summary>
enum AgeDefinition
{
    Unspecified = 0,
    Years,
    Months,
    Weeks,
    Days,
    Hours
};
/// <summary>
/// An Enumration to determine the height definition.
/// </summary>
enum HeightDefinition
{
    Unspecified = 0,
    Centimeters,
    Inches,
    Millimeters
};
/// <summary>
/// An Enumration to determine the weight definition.
/// </summary>
enum WeightDefinition
{
    Unspecified = 0,
    Kilogram,
    Gram,
    Pound,
    Ounce
};
/// <summary>
/// An Enumration to determine the device type.
/// </summary>
enum DeviceType
{
    Cart = 0,
    System
};
/// <summary>
/// An Enumration to determine the device manufactor.
/// </summary>
enum DeviceManufactor
{
    Unknown = 0,
    Burdick,
    Cambridge,
    Compumed,
    Datamed,
    Fukuda,
    HewlettPackard,
    MarquetteElectronics,
    MortaraInstruments,
    NihonKohden,
    Okin,
    Quintin,
    Siemens,
    SpaceLabs,
    Telemed,
    Hellige,
    ESAOTE,
    Schiller,
    PickerSchwarzer,
    ElettronicTrentina,
    Zwonitz,
    Other = 100
};
/// <summary>
/// An Enumration for Electrode Configurations for 12-lead ECG
/// </summary>
enum ElectrodeConfigCodeTwelveLead
{
    Unspecified = 0,
    StandardTwelveLead,
    MasonLikarAndIndividual,
    MasonLikarAndPadded,
    AllLeadPadded,
    TwelveLeadDerivedXYZ,
    TwelveLeadDerivedNonStandard
};
/// <summary>
/// An Enumration for Electrode Configurations for XYZ ECG
/// </summary>
enum ElectrodeConfigCodeXYZ
{
    Unspecified = 0,
    Frank,
    McFeeParungao,
    Cube,
    BipolarUncorrected,
    PseudoOrthogonal,
    XYZDerivedTwelveLead
};
}
