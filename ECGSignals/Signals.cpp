#include "Signals.h"

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
    qrsZone.clear();

    // Signal Data
    _Lead.clear();
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

	_Lead.resize(NrLeads);
}

vector<Signal>& Signals::getLeads()
{
    return _Lead;
}

void Signals::setLeads(const vector<Signal>& leads)
{
    if (leads.size() > uchar_MAX)
        return;

    _Lead = leads;
}

const Signal& Signals::operator[](int i)
{
	return ((_Lead.size() > 0) && (i < _Lead.size())) ? _Lead[i] : Signal();
}

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

#if 0
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

    if (qrsZone.size() != 0)
    {
        //sigs.qrsZone = new QRSZone[qrsZone.size()];
        sigs.qrsZone.resize(qrsZone.size());

        for (int i = 0; i < sigs.qrsZone.size(); i++);
            //sigs.qrsZone[i] = qrsZone[i].Clone(); todo
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
#endif

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

}
}
