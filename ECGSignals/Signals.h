#ifndef _ZQ_SIGNALS_H_
#define _ZQ_SIGNALS_H_
#include "Signal.h"

namespace ECGConversion.ECGSignals
{
/// <summary>
/// Class containing signals of ECG.
/// </summary>
class Signals
{
public:
    Signals();
    Signals(byte nrleads);
    // Overrall Data.
    byte getNrLeads();
    void setNrLeads();

    Signal[] GetLeads();

    void SetLeads(Signal[] leads);

    //		Signal this[int i]


    /// <summary>
    /// Function to determine if the first eigth leads are as expected (I, II, V1 - V6).
    /// </summary>
    /// <returns>true if as expected</returns>
    bool IsNormal();

    /// <summary>
    /// Calculate start and end of signals.
    /// </summary>
    /// <param name="nStart">returns start</param>
    /// <param name="nEnd">returns end</param>
    void CalculateStartAndEnd(out int nStart, out int nEnd);

    /// <summary>
    /// Function to determine the number of simultaneosly.
    /// </summary>
    /// <param name="data">signal information.</param>
    /// <returns>true if as expected</returns>
    int NrSimultaneosly();

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="data">signal array</param>
    void SortOnType();

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="first">first value to sort</param>
    /// <param name="last">last value to sort</param>
    void SortOnType(int first, int last);

    /// <summary>
    /// Function to trim a signals.
    /// </summary>
    /// <param name="val">value to trim on</param>
    void TrimSignals(short val);

    /// <summary>
    /// Function to trim a signals.
    /// </summary>
    /// <param name="val">value to trim on</param>
    /// <param name="start">start of all signals</param>
    /// <param name="end">end of all signals</param>
    void TrimSignals(short val, int start, int end);

    /// <summary>
    /// Function to clone a signals object.
    /// </summary>
    /// <returns>cloned signals object</returns>
    virtual Signals Clone();

    /// <summary>
    /// Function to get a copy of a signals object.
    /// </summary>
    /// <returns>copy to basic signals object</returns>
    Signals GetCopy();

    /// <summary>
    /// Function to make leads a certain length.
    /// </summary>
    /// <param name="seconds">length in seconds</param>
    void MakeSpecificLength(int seconds);

    /// <summary>
    /// Function to make leads a certain length.
    /// </summary>
    /// <param name="seconds">length (in seconds)</param>
    /// <param name="startPoint">start point in signal (in seconds)</param>
    void MakeSpecificLength(int seconds, int startPoint);

    /// <summary>
    /// Function to resample all leads.
    /// </summary>
    /// <param name="samplesPerSecond">samples per second to resample towards</param>
    void Resample(int samplesPerSecond);

    /// <summary>
    /// Set AVM for all signals
    /// </summary>
    /// <param name="avm">preferred multiplier</param>
    void SetAVM(double avm);

    /// <summary>
    /// Determine whether this is twelve lead signal.
    /// </summary>
    /// <returns>true if twelve lead signal.</returns>
    bool IsTwelveLeads();

    /// <summary>
    /// Determine whether this is fifteen lead signal.
    /// </summary>
    /// <returns>true if twelve lead signal.</returns>
    bool IsFifteenLeads();

    /// <summary>
    /// Function to make a twelve leads signals object.
    /// </summary>
    /// <returns>returns twelve leads signals object or null</returns>
    Signals CalculateTwelveLeads();

    /// <summary>
    /// Function to make a fifteen leads signals object.
    /// </summary>
    /// <returns>returns fifteen leads signals object or null</returns>
    Signals CalculateFifteenLeads();
public:
    // Rhythm Info.
    double RhythmAVM; // AVM in uV
    int RhythmSamplesPerSecond;

    // Median Info.
    double MedianAVM; // AVM in uV
    ushort MedianLength; // Length in ms
    int MedianSamplesPerSecond;

    // QRS zones
    ushort MedianFiducialPoint;
    QRSZone[] QRSZone;
private:
    // Signal Data
    private Signal[] _Lead;
};
}


#endif  /*#ifndef _ZQ_SIGNALS_H_*/
