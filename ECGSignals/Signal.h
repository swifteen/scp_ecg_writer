#ifndef _SIGNAL_H_
#define _SIGNAL_H_
#include "ScpGlobal.h"
#include "LeadType.h"
#include <vector>
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
	Signal(const Signal& rhs);		
	Signal& operator=(const Signal& rhs);		
	~Signal();
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
	void deepCopy(const Signal& rhs);
    static int _PartitionOnType(vector<Signal>& data, int first, int last);
public:
    LeadType Type ;
    int RhythmStart;
    int RhythmEnd;

	short* Rhythm;
	int RhythmLength;
//	std::vector<short> Rhythm;
    //short[] Median;
};
}
}

#endif
