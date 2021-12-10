#include "Signals.h"

#include "Signal.h"


namespace ECGConversion{
namespace ECGSignals
{
/// <summary>
/// Class containing signals of ECG.
/// </summary>
Signals::Signals()
{
    // Rhythm Info.
    RhythmAVM = 0; // AVM in uV
    RhythmSamplesPerSecond = 0;

    // Median Info.
    MedianAVM = 0; // AVM in uV
    MedianLength = 0; // Length in ms
    MedianSamplesPerSecond = 0;

    // QRS zones
    MedianFiducialPoint = 0;
    //QRSZone = null;

    // Signal Data
    //_Lead = null;
}

Signals::Signals(uchar nrleads)
{
    NrLeads = nrleads;
}

uchar Signals::getNrLeads()
{
    return (uchar) (_Lead.size() != 0 ? _Lead.size() : 0);
}

void Signals::setNrLeads(uchar NrLeads)
{
if ((NrLeads < uchar_MIN)
||	(NrLeads > uchar_MAX))
return;

//_Lead = new Signal[NrLeads];
_Lead.resize(NrLeads);
}

vector<Signal> Signals::GetLeads()
{
    return _Lead;
}

void Signals::SetLeads(vector<Signal> leads)
{
    if (leads.size() > uchar_MAX)
        return;

    _Lead = leads;
}

#if 0
Signal this[int i]
{
    get
    {
        return ((_Lead != null) && (i < _Lead.Length)) ? _Lead[i] : null;
    }
    set
    {
        _Lead[i] = value;
    }
}
#endif


/// <summary>
/// Function to determine if the first eigth leads are as expected (I, II, V1 - V6).
/// </summary>
/// <returns>true if as expected</returns>
bool Signals::IsNormal()
{
    return Signal::IsNormal(_Lead);
}

/// <summary>
/// Calculate start and end of signals.
/// </summary>
/// <param name="nStart">returns start</param>
/// <param name="nEnd">returns end</param>
void Signals::CalculateStartAndEnd(int &nStart, int &nEnd)
{
    nStart = uint_MAX;
    nEnd = uint_MIN;

    if (_Lead.size() != 0)
    {
        for (int nLead=0;nLead < _Lead.size();nLead++)
        {
            if (_Lead[nLead].RhythmStart < nStart)
                nStart = _Lead[nLead].RhythmStart;

            if (_Lead[nLead].RhythmEnd > nEnd)
                nEnd = _Lead[nLead].RhythmEnd;
        }
    }
}

/// <summary>
/// Function to determine the number of simultaneosly.
/// </summary>
/// <param name="data">signal information.</param>
/// <returns>true if as expected</returns>
int Signals::NrSimultaneosly()
{
    return Signal::NrSimultaneosly(_Lead);
}
/// <summary>
/// Function to sort signal array on lead type.
/// </summary>
/// <param name="data">signal array</param>
void Signals::SortOnType()
{
    Signal::SortOnType(_Lead);
}
/// <summary>
/// Function to sort signal array on lead type.
/// </summary>
/// <param name="first">first value to sort</param>
/// <param name="last">last value to sort</param>
void Signals::SortOnType(int first, int last)
{
    Signal::SortOnType(_Lead, first, last);
}
/// <summary>
/// Function to trim a signals.
/// </summary>
/// <param name="val">value to trim on</param>
void Signals::TrimSignals(short val)
{
    int start, end;

    CalculateStartAndEnd(start, end);

    TrimSignals(val, start, end);
}
/// <summary>
/// Function to trim a signals.
/// </summary>
/// <param name="val">value to trim on</param>
/// <param name="start">start of all signals</param>
/// <param name="end">end of all signals</param>
/*
void Signals::TrimSignals(short val, int start, int end)
{
    foreach (Signal sig in _Lead)
    {
        int	trimBegin = 0,
                trimEnd = sig.Rhythm.size()-1;

        if (sig.RhythmStart == start)
        {
            for (int i=0;i < sig.Rhythm.size();i++)
            {
                if (sig.Rhythm[i] != val)
                {
                    trimBegin = i;
                    break;
                }
            }
        }

        if (sig.RhythmEnd == end)
        {
            for (int i=sig.Rhythm.size()-1;i > 0;i--)
            {
                if (sig.Rhythm[i] != val)
                {
                    trimEnd = i;
                    break;
                }
            }
        }

        if ((trimBegin / RhythmSamplesPerSecond) < 1)
            trimBegin = 0;

        if (((sig.Rhythm.size()-1 - trimEnd) / RhythmSamplesPerSecond) < 1)
            trimEnd = sig.Rhythm.size()-1;

        if ((trimBegin != 0)
                ||	(trimEnd != sig.Rhythm.size()-1))
        {
            sig.RhythmStart += trimBegin;
            sig.RhythmEnd -= (sig.Rhythm.size()-1) - trimEnd;

            short[] temp = new short[trimEnd - trimBegin + 1];

            for (int i=0;i < temp.size();i++)
                temp[i] = sig.Rhythm[i + trimBegin];

            sig.Rhythm = temp;
        }
    }
}

/// <summary>
/// Function to clone a signals object.
/// </summary>
/// <returns>cloned signals object</returns>
virtual Signals Signals::Clone()
{
    Signals sigs;

    sigs.RhythmAVM = RhythmAVM;
    sigs.RhythmSamplesPerSecond = RhythmSamplesPerSecond;

    sigs.MedianAVM = MedianAVM;
    sigs.MedianLength = MedianLength;
    sigs.MedianSamplesPerSecond = MedianSamplesPerSecond;
    sigs.MedianFiducialPoint = MedianFiducialPoint;

    if (qRSZone.size() != 0)
    {
        sigs.qRSZone.resize(qRSZone.size());

        for (int i=0;i < sigs.qRSZone.size();i++)
            //sigs.qRSZone[i] = qRSZone[i].Clone(); todo
    }

    if (_Lead.size() != 0)
    {
        sigs.NrLeads = NrLeads;

        for (int i=0;i < sigs._Lead.size();i++)
            //sigs._Lead[i] = _Lead[i].Clone(); todo
    }

    return sigs;
}
*/

/// <summary>
/// Function to get a copy of a signals object.
/// </summary>
/// <returns>copy to basic signals object</returns>
Signals Signals::GetCopy()
{
    Signals sigs;

    sigs.RhythmAVM = RhythmAVM;
    sigs.RhythmSamplesPerSecond = RhythmSamplesPerSecond;

    sigs.MedianAVM = MedianAVM;
    sigs.MedianLength = MedianLength;
    sigs.MedianSamplesPerSecond = MedianSamplesPerSecond;
    sigs.MedianFiducialPoint = MedianFiducialPoint;

    if (qRSZone.size() != 0)
    {
        //sigs.qRSZone = new QRSZone[qRSZone.size()];
        sigs.qRSZone.resize(qRSZone.size());

        for (int i = 0; i < sigs.qRSZone.size(); i++);
            //sigs.qRSZone[i] = qRSZone[i].Clone(); todo
    }

    if (_Lead.size() != 0)
    {
        sigs.NrLeads = NrLeads;

        for (int i = 0; i < sigs._Lead.size(); i++);
            //sigs._Lead[i] = this._Lead[i].Clone(); todo
    }

    return sigs;
}
/// <summary>
/// Function to make leads a certain length.
/// </summary>
/// <param name="seconds">length in seconds</param>
void Signals::MakeSpecificLength(int seconds)
{
    MakeSpecificLength(seconds, 0);
}
/// <summary>
/// Function to make leads a certain length.
/// </summary>
/// <param name="seconds">length (in seconds)</param>
/// <param name="startPoint">start point in signal (in seconds)</param>

/*
void Signals::MakeSpecificLength(int seconds, int startPoint)
{
    int start, end;

    seconds *= RhythmSamplesPerSecond;
    startPoint *= RhythmSamplesPerSecond;

    CalculateStartAndEnd(start, end);

    foreach (Signal sig in _Lead)
    {
        short[] newSig = new short[seconds+1];

        for (int n=0;n <= seconds;n++)
        {
            int pos = (n - startPoint) + start;

            newSig[n] = ((pos >= sig.RhythmStart) && (pos < sig.RhythmEnd)) ? sig.Rhythm[pos - sig.RhythmStart] : (short) 0;
        }

        sig.Rhythm = newSig;
        sig.RhythmStart = 0;
        sig.RhythmEnd = seconds;
    }
}


/// <summary>
/// Function to resample all leads.
/// </summary>
/// <param name="samplesPerSecond">samples per second to resample towards</param>
void Signals::Resample(int samplesPerSecond)
{
    foreach (Signal sig in this._Lead)
    {
        if ((this.RhythmSamplesPerSecond != 0)
                &&	(this.RhythmAVM != 0)
                &&	(sig.Rhythm != null))
        {
            ECGTool.ResampleLead(sig.Rhythm, this.RhythmSamplesPerSecond, samplesPerSecond, out sig.Rhythm);

            sig.RhythmStart = (int) (((long)sig.RhythmStart * (long)samplesPerSecond) / (long)this.RhythmSamplesPerSecond);
            sig.RhythmEnd = (int) (((long)sig.RhythmEnd * (long)samplesPerSecond) / (long)this.RhythmSamplesPerSecond);
        }

        if ((this.MedianSamplesPerSecond != 0)
                &&	(this.MedianAVM != 0)
                &&	(sig.Median != null))
        {
            ECGTool.ResampleLead(sig.Median, this.MedianSamplesPerSecond, samplesPerSecond, out sig.Median);
        }
    }

    if (this.QRSZone != null)
    {
        foreach (QRSZone zone in this.QRSZone)
        {
            zone.Start = (int) (((long)zone.Start * (long)samplesPerSecond) / (long)this.MedianSamplesPerSecond);
            zone.Fiducial = (int) (((long)zone.Fiducial * (long)samplesPerSecond) / (long)this.MedianSamplesPerSecond);
            zone.End = (int) (((long)zone.End * (long)samplesPerSecond) / (long)this.MedianSamplesPerSecond);
        }
    }

    if ((this.RhythmSamplesPerSecond != 0)
            &&	(this.RhythmAVM != 0))
    {
        this.RhythmSamplesPerSecond = samplesPerSecond;
    }

    if ((this.MedianSamplesPerSecond != 0)
            &&	(this.MedianAVM != 0))
    {
        this.MedianFiducialPoint = (ushort) (((long)this.MedianFiducialPoint * (long)samplesPerSecond) / (long)this.MedianSamplesPerSecond);

        this.MedianSamplesPerSecond = samplesPerSecond;
    }
}
*/

/// <summary>
/// Set AVM for all signals
/// </summary>
/// <param name="avm">preferred multiplier</param>
void Signals::SetAVM(double avm)
{
    if (avm != 0.0)
    {
        int nrLeads = NrLeads;

        for (int i=0;i < nrLeads;i++)
        {
            //ECGTool.ChangeMultiplier(this[i].Rhythm, this.RhythmAVM, avm);todo
            //ECGTool.ChangeMultiplier(this[i].Median, this.MedianAVM, avm);todo
        }

        if (RhythmAVM != 0.0)
            RhythmAVM = avm;

        if (MedianAVM != 0.0)
            MedianAVM = avm;
    }
}
/// <summary>
/// Determine whether this is twelve lead signal.
/// </summary>
/// <returns>true if twelve lead signal.</returns>
bool Signals::IsTwelveLeads()

{
  
	LeadType lt[] = {ECGConversion::ECGSignals::I, II, III, aVR, aVL, aVF, V1, V2, V3, V4, V5, V6};
	int ltSize = sizeof(lt) / sizeof(LeadType);

    int nrSim = NrSimultaneosly();

    if (nrSim != _Lead.size())
        return false;

    if (nrSim == ltSize)
    {
        for (int i=0;i < nrSim;i++)
            if (_Lead[i].Type != lt[i])
                return false;

        return true;
    }
    else if (nrSim == 15)
    {
        int i = 0;

        for (; i < ltSize; i++)
            if (_Lead[i].Type != lt[i])
                return false;

		LeadType extra[][3] = {
			{V7,V3R,V4R},
			{V7,V8,V9},
		};
		int extraSize = sizeof(extra)/sizeof(extra[0]);// => 2
			
		//vector<bool> check(extraSize, true);
		bool check[2];
		
        for (int c=0;c<2;c++)
            check[c] = true;
       
        for (int j = 0; i < nrSim; i++, j++)
            for (int c = 0; c < extraSize; c++)
                check[c] &= (_Lead[i].Type == extra[c][j]);

        for (i = 0; i < 2; i++)
            if (check[i])
                return true;
    }

    return false;
}
/// <summary>
/// Determine whether this is fifteen lead signal.
/// </summary>
/// <returns>true if twelve lead signal.</returns>
bool Signals::IsFifteenLeads()
{
		LeadType lts[][15] = {
					{ I, II, III,
                    aVR, aVL, aVF,
                    V1, V2, V3,
                    V4, V5, V6,
                    V7, V3R, V4R 
                    },
					{I, II, III,
                    aVR, aVL, aVF,
                    V1, V2, V3,
                    V4, V5, V6,
                    V7, V8, V9 }
					};

	int ltsSize = sizeof(lts)/sizeof(lts[0]);// => 2
	
    int nrSim = NrSimultaneosly();

	
    if (nrSim != _Lead.size())
        return false;

    if (nrSim == 15)
    {
        bool check[2];

        for (int c = 0; c < 2; c++)
            check[c] = true;

        for (int i = 0; i < nrSim; i++)
            for (int c = 0; c < ltsSize; c++)
                check[c] &= (_Lead[i].Type == lts[c][i]);

        for (int i = 0; i < 2; i++)
            if (check[i])
                return true;
    }

    return false;
}
/// <summary>
/// Function to make a twelve leads signals object.
/// </summary>
/// <returns>returns twelve leads signals object or null</returns>

Signals Signals::CalculateTwelveLeads()
{

	LeadType lt[] = {I, II, III,
                aVR, aVL, aVF,
                V1, V2, V3,
                V4, V5, V6};
	int ltSize = sizeof(lt) / sizeof(LeadType);
    int nrSim = NrSimultaneosly();

    if (nrSim != _Lead.size())
        return null;

    Signal[] leads = null;

    if (nrSim == 12)
    {
        //ArrayList pos_list = new ArrayList(lt);
		vector<LeadType> pos_list(lt,lt + ltSize);
        int check_one = 0;
        //ArrayList check_two = new ArrayList(lt);
        vector<LeadType> check_two(lt,lt + ltSize);
        //Signal[] pos = new Signal[12];
		vector<Signal> pos(12);
        for (int i=0;i < nrSim;i++)
        {
            if (_Lead[i].Type == lt[i])
                check_one++;

            int temp = check_two.IndexOf(_Lead[i].Type);
            if (temp < 0)
                return null;

            check_two.RemoveAt(temp);

            pos[pos_list.IndexOf(_Lead[i].Type)] = _Lead[i];
        }

        if (check_one == 12)
            return *this;

        if (check_two.Count == 0)
        {
            //for (int i=0;i < pos.size();i++)
                //if (pos[i] != null)
                    //pos[i] = pos[i].Clone();todo

            leads = pos;
        }
    }
    else
    {
        short[][]
                tempRhythm = null,
                tempMedian = null;

        Signal[] pos = new Signal[12];

        if (nrSim == 8)
        {
            //ArrayList pos_list = new ArrayList(lt);
			vector<LeadType> pos_list(lt, lt + ltSize);
	
            ArrayList check = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6});

            for (int i=0;i < nrSim;i++)
            {
                int temp = check.IndexOf(_Lead[i].Type);
                if (temp < 0)
                    return null;

                check.RemoveAt(temp);

                pos[pos_list.IndexOf(_Lead[i].Type)] = _Lead[i];
            }

            if (check.Count == 0)
            {
                for (int i=0;i < pos.size();i++)
                    if (pos[i] != null)
                        pos[i] = pos[i].Clone();

                tempRhythm = new short[2][];
                tempRhythm[0] = pos[0].Rhythm;
                tempRhythm[1] = pos[1].Rhythm;

                tempMedian = new short[2][];
                tempMedian[0] = pos[0].Median;
                tempMedian[1] = pos[1].Median;
            }
        }
        else if (nrSim == 9)
        {
            ArrayList pos_list = new ArrayList(lt);

            ArrayList check = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II, LeadType.III,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6});

            for (int i=0;i < nrSim;i++)
            {
                int temp = check.IndexOf(_Lead[i].Type);
                if (temp < 0)
                    return null;

                check.RemoveAt(temp);

                pos[pos_list.IndexOf(_Lead[i].Type)] = _Lead[i];
            }

            if (check.Count == 0)
            {
                for (int i=0;i < pos.size();i++)
                    if (pos[i] != null)
                        pos[i] = pos[i].Clone();

                tempRhythm = new short[3][];
                tempRhythm[0] = pos[0].Rhythm;
                tempRhythm[1] = pos[1].Rhythm;
                tempRhythm[2] = pos[2].Rhythm;

                tempMedian = new short[3][];
                tempMedian[0] = pos[0].Median;
                tempMedian[1] = pos[1].Median;
                tempMedian[2] = pos[2].Median;
            }
        }

        if ((tempRhythm != null)
                ||	(tempMedian != null))
        {
            short[][] calcLeads;

            if ((tempRhythm != null)
                    &&	(tempRhythm[0] != null)
                    &&	ECGTool.CalculateLeads(tempRhythm, tempRhythm[0].size(), out calcLeads) == 0)
            {
                for (int i=0;i < calcLeads.size();i++)
                {
                    Signal sig = new Signal();
                    sig.Type = lt[i + tempRhythm.size()];
                    sig.RhythmStart	= pos[0].RhythmStart;
                    sig.RhythmEnd	= pos[0].RhythmEnd;
                    sig.Rhythm = calcLeads[i];

                    pos[i + tempRhythm.size()] = sig;
                }

                if ((tempMedian != null)
                        &&	(tempMedian[0] != null)
                        &&	(ECGTool.CalculateLeads(tempMedian, tempMedian[0].size(), out calcLeads) == 0))
                {
                    for (int i=0;i < calcLeads.size();i++)
                    {
                        pos[i + tempRhythm.size()].Median = calcLeads[i];
                    }
                }

                leads = pos;
            }
        }
    }

    if (leads != null)
    {
        //Signals sigs = this.Clone(); todo

        sigs.NrLeads = (byte) leads.size();

        for (int i=0;i < leads.size();i++)
            sigs._Lead[i] = leads[i];

        return sigs;
    }

    return null;
}

/// <summary>
/// Function to make a fifteen leads signals object.
/// </summary>
/// <returns>returns fifteen leads signals object or null</returns>
/*
Signals Signals::CalculateFifteenLeads()
{
    LeadType[] lt1 = new LeadType[] { LeadType.I, LeadType.II, LeadType.III,
                LeadType.aVR, LeadType.aVL, LeadType.aVF,
                LeadType.V1, LeadType.V2, LeadType.V3,
                LeadType.V4, LeadType.V5, LeadType.V6,
                LeadType.V7, LeadType.V3R, LeadType.V4R };
    LeadType[] lt2 =  new LeadType[] { LeadType.I, LeadType.II, LeadType.III,
                LeadType.aVR, LeadType.aVL, LeadType.aVF,
                LeadType.V1, LeadType.V2, LeadType.V3,
                LeadType.V4, LeadType.V5, LeadType.V6,
                LeadType.V7, LeadType.V8, LeadType.V9 };

    int nrSim = NrSimultaneosly();

    if (nrSim != _Lead.size())
        return null;

    Signal[] leads = null;

    if (nrSim == lt1.size())
    {
        ArrayList pos_list1 = new ArrayList(lt1);
        ArrayList pos_list2 = new ArrayList(lt2);

        int check_one1 = 0;
        int check_one2 = 0;
        ArrayList check_two1 = new ArrayList(lt1);
        ArrayList check_two2 = new ArrayList(lt2);
        Signal[] pos1 = new Signal[lt1.size()];
        Signal[] pos2 = new Signal[lt2.size()];

        for (int i = 0; i < nrSim; i++)
        {
            if (_Lead[i].Type == lt1[i])
                check_one1++;

            if (_Lead[i].Type == lt2[i])
                check_one2++;

            int temp = check_two1.IndexOf(_Lead[i].Type);
            if (temp >= 0)
            {
                check_two1.RemoveAt(temp);

                pos1[pos_list1.IndexOf(_Lead[i].Type)] = _Lead[i];
            }
            temp = check_two2.IndexOf(_Lead[i].Type);
            if (temp >= 0)
            {
                check_two2.RemoveAt(temp);

                pos2[pos_list2.IndexOf(_Lead[i].Type)] = _Lead[i];
            }
        }

        if (check_one1 == lt1.size())
            return this;
        if (check_one2 == lt2.size())
            return this;

        if (check_two1.Count == 0)
        {
            for (int i = 0; i < pos1.size(); i++)
                if (pos1[i] != null)
                    pos1[i] = pos1[i].Clone();

            leads = pos1;
        }
        else if (check_two2.Count == 0)
        {
            for (int i = 0; i < pos2.size(); i++)
                if (pos2[i] != null)
                    pos2[i] = pos2[i].Clone();

            leads = pos2;
        }
    }
    else
    {
        LeadType[] lt = null;

        short[][]
                tempRhythm = null,
                tempMedian = null;

        Signal[] pos = null;

        if (nrSim == 11)
        {
            Signal[] pos1 = new Signal[lt1.size()];
            Signal[] pos2 = new Signal[lt2.size()];

            ArrayList pos_list1 = new ArrayList(lt1);
            ArrayList pos_list2 = new ArrayList(lt2);

            ArrayList check1 = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6,
                        LeadType.V7, LeadType.V3R, LeadType.V4R});

            ArrayList check2 = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6,
                        LeadType.V7, LeadType.V8, LeadType.V9});

            for (int i = 0; i < nrSim; i++)
            {
                int temp = check1.IndexOf(_Lead[i].Type);
                if (temp >= 0)
                {
                    check1.RemoveAt(temp);

                    pos1[pos_list1.IndexOf(_Lead[i].Type)] = _Lead[i];
                }
                temp = check2.IndexOf(_Lead[i].Type);
                if (temp >= 0)
                {
                    check2.RemoveAt(temp);

                    pos2[pos_list2.IndexOf(_Lead[i].Type)] = _Lead[i];
                }
            }

            if (check1.Count == 0)
            {
                pos = pos1;
                lt = lt1;
            }
            else if (check2.Count == 0)
            {
                pos = pos2;
                lt = lt2;
            }

            if (pos != null)
            {
                for (int i = 0; i < pos.size(); i++)
                    if (pos[i] != null)
                        pos[i] = pos[i].Clone();

                tempRhythm = new short[2][];
                tempRhythm[0] = pos1[0].Rhythm;
                tempRhythm[1] = pos1[1].Rhythm;

                tempMedian = new short[2][];
                tempMedian[0] = pos1[0].Median;
                tempMedian[1] = pos1[1].Median;
            }
        }
        else if (nrSim == 12)
        {
            Signal[] pos1 = new Signal[lt1.size()];
            Signal[] pos2 = new Signal[lt2.size()];

            ArrayList pos_list1 = new ArrayList(lt1);
            ArrayList pos_list2 = new ArrayList(lt2);

            ArrayList check1 = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II, LeadType.III,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6,
                        LeadType.V7, LeadType.V3R, LeadType.V4R});

            ArrayList check2 = new ArrayList(
                        new LeadType[]{	LeadType.I, LeadType.II, LeadType.III,
                        LeadType.V1, LeadType.V2, LeadType.V3,
                        LeadType.V4, LeadType.V5, LeadType.V6,
                        LeadType.V7, LeadType.V8, LeadType.V9});

            for (int i = 0; i < nrSim; i++)
            {
                int temp = check1.IndexOf(_Lead[i].Type);
                if (temp >= 0)
                {
                    check1.RemoveAt(temp);

                    pos1[pos_list1.IndexOf(_Lead[i].Type)] = _Lead[i];
                }
                temp = check2.IndexOf(_Lead[i].Type);
                if (temp >= 0)
                {
                    check2.RemoveAt(temp);

                    pos2[pos_list2.IndexOf(_Lead[i].Type)] = _Lead[i];
                }
            }

            if (check1.Count == 0)
            {
                pos = pos1;
                lt = lt1;
            }
            else if (check2.Count == 0)
            {
                pos = pos2;
                lt = lt2;
            }

            if (pos != null)
            {
                for (int i = 0; i < pos.size(); i++)
                    if (pos[i] != null)
                        pos[i] = pos[i].Clone();

                tempRhythm = new short[3][];
                tempRhythm[0] = pos[0].Rhythm;
                tempRhythm[1] = pos[1].Rhythm;
                tempRhythm[2] = pos[2].Rhythm;

                tempMedian = new short[3][];
                tempMedian[0] = pos[0].Median;
                tempMedian[1] = pos[1].Median;
                tempMedian[2] = pos[2].Median;
            }
        }

        if ((tempRhythm != null)
                ||  (tempMedian != null))
        {
            short[][] calcLeads;

            if ((tempRhythm != null)
                    &&  (tempRhythm[0] != null)
                    &&  ECGTool.CalculateLeads(tempRhythm, tempRhythm[0].size(), out calcLeads) == 0)
            {
                for (int i = 0; i < calcLeads.size(); i++)
                {
                    Signal sig = new Signal();
                    sig.Type = lt[i + tempRhythm.size()];
                    sig.RhythmStart = pos[0].RhythmStart;
                    sig.RhythmEnd = pos[0].RhythmEnd;
                    sig.Rhythm = calcLeads[i];

                    pos[i + tempRhythm.size()] = sig;
                }

                if ((tempMedian != null)
                        && (tempMedian[0] != null)
                        && (ECGTool.CalculateLeads(tempMedian, tempMedian[0].size(), out calcLeads) == 0))
                {
                    for (int i = 0; i < calcLeads.size(); i++)
                    {
                        pos[i + tempRhythm.size()].Median = calcLeads[i];
                    }
                }

                leads = pos;
            }
        }
    }

    if (leads != null)
    {
        //Signals sigs = this.Clone();todo

        sigs.size() = (byte)leads.size();

        for (int i = 0; i < leads.size(); i++)
            sigs._Lead[i] = leads[i];

        return sigs;
    }

    return null;
}
*/

}
}
