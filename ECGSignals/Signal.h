#ifndef _SIGNAL_H_
#define _SIGNAL_H_

namespace ECGConversion.ECGSignals
{
/// <summary>
/// Class Containing data of one signal.
/// </summary>
class Signal
{
public:
	public Signal Clone();
		/// <summary>
	/// Function to apply a bandpass filter on Signal object
	/// </summary>
	/// <param name="rhythmFilter">Provide filter for rhythm data</param>
	/// <param name="medianFilter">Provide filter for median data</param>
	/// <returns>a filtered copy of object</returns>
	public Signal ApplyFilter(DSP.IFilter rhythmFilter, DSP.IFilter medianFilter);
	
    /// <summary>
    /// Function to determine if the first eigth leads are as expected (I, II, V1 - V6).
    /// </summary>
    /// <param name="data">signal information.</param>
    /// <returns>true if as expected</returns>
    static bool IsNormal(Signal[] data);

    /// <summary>
    /// Function to determine the number of simultaneosly.
    /// </summary>
    /// <param name="data">signal information.</param>
    /// <returns>true if as expected</returns>
    static int NrSimultaneosly(Signal[] data);

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="data">signal array</param>
    static void SortOnType(Signal[] data);

    /// <summary>
    /// Function to sort signal array on lead type.
    /// </summary>
    /// <param name="data">signal array</param>
    /// <param name="first"></param>
    /// <param name="last"></param>
    static void SortOnType(Signal[] data, int first, int last);

private:
    static int _PartitionOnType(Signal[] data, int first, int last);
public:
    LeadType Type ;
    int RhythmStart;
    int RhythmEnd;
    short[] Rhythm;
    short[] Median;
};

}
