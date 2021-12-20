#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_


namespace ECGConversion.ECGLeadMeasurements
{
/// <summary>
/// Enumration Morphology for lead measurements.
/// </summary>
enum Morphology {
    Unknown = 0,
    Positive,
    Negative,
    PositiveNegative,
    NegativePositive,
    PositiveNegativePositive,
    NegativePositiveNegative,
    NotchedMShaped,
    NotchedWShaped
};
}
