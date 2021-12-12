#include "Signal.h"

namespace ECGConversion{

namespace ECGSignals
{
/// <summary>
/// Class Containing data of one signal.
/// </summary>
Signal::Signal()
{
    Type = LeadTypeUnknown;
    RhythmStart = 0;
    RhythmEnd = 0;
    Rhythm = null;
	RhythmLength = 0;
    //Median = null;
}
/// <summary>
/// Function to make a deep copy of this object.
/// </summary>
/// <returns>copy of object</returns>
Signal::Signal(const Signal& rhs)
{
	deepCopy(rhs);
}

Signal& Signal::operator=(const Signal& rhs)
{
	// Prevent self-assignment
	if( &rhs != this )
	{
		delete[] this->Rhythm;
		this->Rhythm = null;		
		RhythmLength = 0;
		deepCopy(rhs);
	}
	return *this;
}

Signal::~Signal()
{
	delete[] Rhythm;
}

void Signal::deepCopy(const Signal& rhs)
{
	this->Type = rhs.Type;
	this->RhythmStart = rhs.RhythmStart;
	this->RhythmEnd = rhs.RhythmEnd;
	this->RhythmLength = rhs.RhythmLength;
	if((rhs.RhythmLength > 0) && (rhs.Rhythm != null))
	{
		this->Rhythm = new short[rhs.RhythmLength];
		if(this->Rhythm != null)
		{
			memcpy(this->Rhythm,rhs.Rhythm,rhs.RhythmLength);
		}
	}
}
/// <summary>
/// Function to apply a bandpass filter on Signal object
/// </summary>
/// <param name="rhythmFilter">Provide filter for rhythm data</param>
/// <param name="medianFilter">Provide filter for median data</param>
/// <returns>a filtered copy of object</returns>
#if 0 
Signal Signal::ApplyFilter(DSP.IFilter rhythmFilter, DSP.IFilter medianFilter)
{
	Signal sig = new Signal();

	sig.Type = this.Type;
	sig.RhythmStart = this.RhythmStart;
	sig.RhythmEnd = this.RhythmEnd;

	if (this.Rhythm != null)
	{
		if (rhythmFilter == null)
			return null;

		rhythmFilter.compute(this.Rhythm[0]);
		rhythmFilter.compute(this.Rhythm[0]);

		sig.Rhythm = new short[this.Rhythm.Length];
		for (int i = 0; i < sig.Rhythm.Length; i++)
			sig.Rhythm[i] = (short) Math.Round(rhythmFilter.compute(this.Rhythm[i]));
	}

	if (this.Median != null)
	{
		if (medianFilter == null)
			return null;

		medianFilter.compute(this.Median[0]);
		medianFilter.compute(this.Median[0]);

		sig.Median = new short[this.Median.Length];
		for (int i = 0; i < sig.Median.Length; i++)
			sig.Median[i] = (short) Math.Round(medianFilter.compute(this.Median[i]));
	}

	return sig;
}
#endif

/// <summary>
/// Function to determine if the first eigth leads are as expected (I, II, V1 - V6).
/// </summary>
/// <param name="data">signal information.</param>
/// <returns>true if as expected</returns>
bool Signal::IsNormal(const vector<Signal>& data)
{
    if ((data.size() != 0)
            &&	(data.size() >= 8))
    {
        for (int loper=0;loper < 8;loper++)
        {
            if ((data[loper].Type != (LeadType) (1 + loper)))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
/// <summary>
/// Function to determine the number of simultaneosly.
/// </summary>
/// <param name="data">signal information.</param>
/// <returns>true if as expected</returns>
int Signal::NrSimultaneosly(const vector<Signal>& data)
{
    if ((data.size() != 0)
            &&  (data.size() > 1))
    {
        int Nr = 1;
        for (;Nr < data.size();Nr++)
        {
           
            if ((data[0].RhythmStart != data[Nr].RhythmStart)
                    ||	(data[0].RhythmEnd != data[Nr].RhythmEnd))
            {
                break;
            }
        }
        return Nr;
    }
    return 0;
}
/// <summary>
/// Function to sort signal array on lead type.
/// </summary>
/// <param name="data">signal array</param>
void Signal::SortOnType(vector<Signal>& data)
{
    if (data.size() != 0)
    {
        SortOnType(data, 0, data.size()-1);
    }
}
/// <summary>
/// Function to sort signal array on lead type.
/// </summary>
/// <param name="data">signal array</param>
/// <param name="first"></param>
/// <param name="last"></param>
void Signal::SortOnType(vector<Signal>& data, int first, int last)
{
    if ((data.size() != 0)
           && (first < last))
    {
        int p = _PartitionOnType(data, first, last);

        SortOnType(data, first, p - 1);
        SortOnType(data, p + 1, last);
    }
}
int Signal::_PartitionOnType(vector<Signal>& data, int first, int last)
{
    Signal pivot, t;
    int i, m, p;

    m = (first + last) / 2;

    pivot = data[m];
    data[m] = data[first];
    data[first] = pivot;

    p = first;

    for (i=first+1;i <= last;i++)
    {
        if ((data.size() == 0)
                ||	(data[i].Type < pivot.Type))
        {
            t = data[++p];
            data[p] = data[i];
            data[i] = t;
        }
    }

    t = data[first];
    data[first] = data[p];
    data[p] = t;

    return p;
}

}
}
