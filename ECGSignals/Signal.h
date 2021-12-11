#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#include "ScpGlobal.h"
#include "LeadType.h"
using namespace std;

namespace ECGConversion{

namespace ECGSignals
{
/// <summary>
/// Class Containing data of one signal.
/// </summary>
class Signal
{
public:
	Signal(); 
#if 0 //todo
	Signal Clone();
		/// <summary>
	/// Function to apply a bandpass filter on Signal object
	/// </summary>
	/// <param name="rhythmFilter">Provide filter for rhythm data</param>
	/// <param name="medianFilter">Provide filter for median data</param>
	/// <returns>a filtered copy of object</returns>
	Signal ApplyFilter(DSP.IFilter rhythmFilter, DSP.IFilter medianFilter);
#endif
    /// <summary>
    /// Function to determine if the first eigth leads are as expected (I, II, V1 - V6).
    /// </summary>
    /// <param name="data">signal information.</param>
    /// <returns>true if as expected</returns>
    static bool IsNormal(const vector<Signal>& data);

    /// <summary>
    /// Function to determine the number of simultaneosly.
    /// </summary>
    /// <param name="data">signal information.</param>
    /// <returns>true if as expected</returns>
    static int NrSimultaneosly(const vector<Signal>& data);

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="data">signal array</param>
    static void SortOnType(vector<Signal>& data);

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="data">signal array</param>
    /// <param name="first"></param>
    /// <param name="last"></param>
    static void SortOnType(vector<Signal>& data, int first, int last);

private:
    static int _PartitionOnType(vector<Signal>& data, int first, int last);
public:
    LeadType Type ;
    int RhythmStart;
    int RhythmEnd;
	
    //short[] Rhythm;
    //short[] Median;
};
}
}

#endif
