#ifndef _SIGNALS_H_
#define _SIGNALS_H_
#include "ScpGlobal.h"
#include <vector>
#include "Signal.h"
#include "LeadType.h"
#include "QRSZone.h"
using namespace std;


namespace ECGConversion {

namespace ECGSignals
{
/// <summary>
/// Class containing signals of ECG.
/// </summary>
class Signals
{
public:
    Signals();
    Signals(uchar nrleads);
    // Overrall Data.
    uchar getNrLeads();
    //void setNrLeads();
	void setNrLeads(uchar NrLeads);

    vector<Signal>& getLeads();
    void setLeads(const vector<Signal>& leads);

	const Signal& operator[](int i);

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
    void CalculateStartAndEnd(int &nStart, int &nEnd);

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

#if 0
    /// <summary>
    /// Function to get a copy of a signals object.
    /// </summary>
    /// <returns>copy to basic signals object</returns>
    Signals GetCopy();
    /// <summary>
    /// Function to resample all leads.
    /// </summary>
    /// <param name="samplesPerSecond">samples per second to resample towards</param>
    void Resample(int samplesPerSecond);
#endif

    /// <summary>
    /// Set AVM for all signals
    /// </summary>
    /// <param name="avm">preferred multiplier</param>
    void SetAVM(double avm);

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
    vector<QRSZone> qrsZone;
private:
    // Signal Data
    vector<Signal> _Lead;
	uchar NrLeads;
};
}
}

#endif  /*#ifndef _SIGNALS_H_*/
