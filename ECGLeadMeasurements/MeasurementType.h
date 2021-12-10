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
    None = -1,
    Pdur = 0,
    PRint,
    QRSdur,
    QTint,
    Qdur,
    Rdur,
    Sdur,
    RRdur,
    SSdur,
    RRRdur,
    Qamp,
    Ramp,
    Samp,
    RRamp,
    SSamp,
    RRRamp,
    Jamp,
    Pamp_pos,
    Pamp_min,
    Tamp_pos,
    Tamp_min,
    STslope,
    Pmorphology,
    Tmorphology,
    IsoElectricQRSonset,
    IsoElectricQRSend,
    IntrinsicoidDeflection,
    QualityCode,
    STampJ20,
    STampJ60,
    STampJ80,
    STamp1_16RR,
    STamp1_8RR,
    QRSonset,
    QRSoffset,
    Qoffset,
    Roffset,
    Soffset,
    RRoffset,
    SSoffset,
    RRRoffset,
    Toffset,
    Pnotch,
    Rnotch,

    Ronset = Qoffset,
    Sonset =  Roffset,
    RRonset = Soffset,
    SSonset = RRoffset,
    RRRonset = SSoffset,
};



}
}


#endif  /*#ifndef _MEASUREMENTTYPE_H_*/
