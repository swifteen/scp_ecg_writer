#ifndef _MEASUREMENTTYPE_H_
#define _MEASUREMENTTYPE_H_
    

namespace ECGConversion
{

namespace ECGLeadMeasurements
{
/// <summary>
/// Enumration MeasurementType for lead measurements.
/// </summary>
enum MeasurementType
{
    MeasurementTypeNone = -1,
    MeasurementTypePdur = 0,
    MeasurementTypePRint,
    MeasurementTypeQRSdur,
    MeasurementTypeQTint,
    MeasurementTypeQdur,
    MeasurementTypeRdur,
    MeasurementTypeSdur,
    MeasurementTypeRRdur,
    MeasurementTypeSSdur,
    MeasurementTypeRRRdur,
    MeasurementTypeQamp,
    MeasurementTypeRamp,
    MeasurementTypeSamp,
    MeasurementTypeRRamp,
    MeasurementTypeSSamp,
    MeasurementTypeRRRamp,
    MeasurementTypeJamp,
    MeasurementTypePamp_pos,
    MeasurementTypePamp_min,
    MeasurementTypeTamp_pos,
    MeasurementTypeTamp_min,
    MeasurementTypeSTslope,
    MeasurementTypePmorphology,
    MeasurementTypeTmorphology,
    MeasurementTypeIsoElectricQRSonset,
    MeasurementTypeIsoElectricQRSend,
    MeasurementTypeIntrinsicoidDeflection,
    MeasurementTypeQualityCode,
    MeasurementTypeSTampJ20,
    MeasurementTypeSTampJ60,
    MeasurementTypeSTampJ80,
    MeasurementTypeSTamp1_16RR,
    MeasurementTypeSTamp1_8RR,
    MeasurementTypeQRSonset,
    MeasurementTypeQRSoffset,
    MeasurementTypeQoffset,
    MeasurementTypeRoffset,
    MeasurementTypeSoffset,
    MeasurementTypeRRoffset,
    MeasurementTypeSSoffset,
    MeasurementTypeRRRoffset,
    MeasurementTypeToffset,
    MeasurementTypePnotch,
    MeasurementTypeRnotch,

    MeasurementTypeRonset = MeasurementTypeQoffset,
    MeasurementTypeSonset =  MeasurementTypeRoffset,
    MeasurementTypeRRonset = MeasurementTypeSoffset,
    MeasurementTypeSSonset = MeasurementTypeRRoffset,
    MeasurementTypeRRRonset = MeasurementTypeSSoffset,
    MeasurementTypeSum,
};



}
}


#endif  /*#ifndef _MEASUREMENTTYPE_H_*/
