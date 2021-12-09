#include "SCPSection6.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion.SCP
{


/// <summary>
/// Class contains section 6 (Rhythm data section).
/// </summary>

ushort SCPSection6::_SectionID = 6;


SCPSection6::SCPSection6()
{


    // special variable for setting nr leads before a read.
    ushort _NrLeads = 0;

    // Part of the stored Data Structure.
    ushort _AVM = 0;
    ushort _TimeInterval = 0;
    byte _Difference = 0;
    byte _Bimodal = 0;
    ushort[] _DataLength = null;
    byte[][] _Data = null;
}

int SCPSection6::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_AVM, buffer, offset, sizeof(_AVM), true);
    offset += sizeof(_AVM);
    BytesTool::writeBytes(_TimeInterval, buffer, offset, sizeof(_TimeInterval), true);
    offset += sizeof(_TimeInterval);
    BytesTool::writeBytes(_Difference, buffer, offset, sizeof(_Difference), true);
    offset += sizeof(_Difference);
    BytesTool::writeBytes(_Bimodal, buffer, offset, sizeof(_Bimodal), true);
    offset += sizeof(_Bimodal);

    int offset2 = offset + (_Data.Length * sizeof(_DataLength[0]));

    for (int loper=0;loper < _Data.Length;loper++)
    {
        BytesTool::writeBytes(_DataLength[loper], buffer, offset, sizeof(_DataLength[loper]), true);
        offset += sizeof(_DataLength[loper]);
        BytesTool::copy(buffer, offset2, _Data[loper], 0, _Data[loper].Length);
        offset2 += _DataLength[loper];
    }
    return 0x00;
}
void SCPSection6::_Empty()
{
    _AVM = 0;
    _TimeInterval = 0;
    _Difference = 0;
    _Bimodal = 0;
    _DataLength = null;
    _Data = null;
}
int SCPSection6::_getLength()
{
    if (Works())
    {
        int sum = sizeof(_AVM) + sizeof(_TimeInterval) + sizeof(_Difference) + sizeof(_Bimodal);
        sum += (_Data.Length * sizeof(_DataLength[0]));
        for (int loper=0;loper < _Data.Length;loper++)
        {
            sum += _DataLength[loper];
        }
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}
ushort SCPSection6::getSectionID()
{
    return _SectionID;
}
bool SCPSection6::Works()
{
    if ((_Data != null)
            &&  (_DataLength != null)
            &&  (_Data.Length == _DataLength.Length))
    {
        for (int loper=0;loper < _Data.Length;loper++)
        {
            if ((_Data[loper] == null)
                    ||	(_DataLength[loper] < _Data[loper].Length))
            {
                return false;
            }
        }
        return true;
    }
    return false;
}
/// <summary>
/// Function to set nr of leads used in section (Solution for a tiny problem).
/// </summary>
/// <param name="nrleads">nr of leads in section</param>
void SCPSection6::setNrLeads(ushort nrleads)
{
    _NrLeads = nrleads;
}
/// <summary>
/// Function to encode data in this section.
/// </summary>
/// <param name="data">Rhythm data to encode</param>
/// <param name="tables">Huffman table to use during enconding</param>
/// <param name="leadDefinition">Lead Definitions to use for encoding</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>0 on succes</returns>
int SCPSection6::EncodeData(short[][] data, SCPSection2 tables, SCPSection3 leadDefinition, SCPSection4 qrsLocations, int medianFreq, byte difference)
{
    int localFreq = getSamplesPerSecond();

    if ((data != null)
            &&	(tables != null)
            &&	(leadDefinition != null)
            &&  (localFreq > 0))
    {
        if ((medianFreq <= 0)
                ||  (medianFreq == localFreq))
        {
            medianFreq = 1;
            localFreq = 1;
        }

        if ((_Bimodal == 0x1)
                &&  (qrsLocations == null))
        {
            return 2;
        }

        ushort nrleads = leadDefinition.getNrLeads();
        _Data = new byte[nrleads][];
        _DataLength = new ushort[nrleads];
        for (int loper=0;loper < nrleads;loper++)
        {
            if (data[loper] == null)
            {
                return 4;
            }

            short[] temp = data[loper];

            int time = (leadDefinition.getLeadLength(loper) * localFreq) / medianFreq;
            if (localFreq != medianFreq)
            {
                int rate = (medianFreq / localFreq);
                // Bimodal part might be buggy unable to test.
                if ((_Bimodal == 0x1)
                        &&	((medianFreq % localFreq) == 0)
                        &&	(rate > 0)
                        &&	(rate < 5))
                {
                    // Calculate nr of samples stored in section.
                    time = 0;

                    int nrzones = qrsLocations.getNrProtectedZones();
                    for (int zone=0;zone < nrzones;zone++)
                    {
                        int begin = (qrsLocations.getProtectedStart(zone) >= leadDefinition.getLeadStart(loper) ? qrsLocations.getProtectedStart(zone) : leadDefinition.getLeadStart(loper));
                        int end = (qrsLocations.getProtectedEnd(zone) <= leadDefinition.getLeadEnd(loper) ? qrsLocations.getProtectedEnd(zone) : leadDefinition.getLeadEnd(loper));

                        begin = (end > begin ? end - begin + 1 : 0);

                        time += begin + (rate - (begin % rate));
                    }

                    time += ((leadDefinition.getLeadLength(loper) - time) * localFreq) / medianFreq;

                    int leadLength = leadDefinition.getLeadLength(loper);

                    time += ((leadLength - time) * localFreq) / medianFreq;

                    // Restructure bimodal data to length set in Section3.
                    temp = new short[time];

                    int time2Offset = leadDefinition.getLeadStart(loper);
                    int time1=0;
                    int time2=0;

                    while ((time1 < temp.Length)
                           && (time2 <= leadLength)
                           && (time2 < data[loper].Length))
                    {
                        int zone=0;
                        int end = qrsLocations.getNrProtectedZones();
                        for (;zone < end;zone++)
                        {
                            if ((qrsLocations.getProtectedLength(zone) > 0)
                                    &&	(time2 + time2Offset >= qrsLocations.getProtectedStart(zone))
                                    &&  (time2 + time2Offset <= qrsLocations.getProtectedEnd(zone)))
                            {
                                break;
                            }
                        }

                        if (zone < end)
                        {
                            temp[time1] = data[loper][time2++];
                        }
                        else
                        {
                            int Sum = 0;
                            for (int sumLoper=0;sumLoper < rate;sumLoper++)
                            {
                                Sum += data[loper][time2 + sumLoper];
                            }
                            temp[time1] = (short) (Sum / rate);
                            time2 += rate;
                        }

                        time1++;
                    }
                }
                else
                {
                    _Bimodal = 0;
                    ECGTool.ResampleLead(temp, medianFreq, localFreq, out temp);
                }
            }

            _Difference = difference;
            _Data[loper] = tables.Encode(temp, time, 0, _Difference);
            if (_Data[loper] == null)
            {
                _Data = null;
                _DataLength = null;
                return 8;
            }
            _DataLength[loper] = (ushort) _Data[loper].Length;
            if ((_DataLength[loper] & 0x1) == 0x1)
            {
                _DataLength[loper]++;
            }
        }
        return 0;
    }
    return 1;
}
/// <summary>
/// Function to get AVM.
/// </summary>
/// <returns>AVM in uV</returns>
double SCPSection6::getAVM()
{
    if (_AVM > 0)
    {
        return (((double)_AVM) / 1000.0);
    }
    return -1;
}
/// <summary>
/// Function to set AVM.
/// </summary>
/// <param name="avm">AVM in uV</param>
void  SCPSection6::setAVM(double avm)
{
    if (avm > 0)
    {
        _AVM  = (ushort) (avm * 1000);
    }
}
/// <summary>
/// Function to get samples per second used in data.
/// </summary>
/// <returns>samples per second</returns>
int SCPSection6::getSamplesPerSecond()
{
    if (_TimeInterval > 0)
    {
        return (1000000 / _TimeInterval);
    }
    return -1;
}
/// <summary>
/// Function to set samples per second used in data.
/// </summary>
/// <param name="sps">samples per second</param>
void SCPSection6::setSamplesPerSecond(int sps)
{
    if (sps > 0)
    {
        _TimeInterval = (ushort) (1000000 / sps);
    }
}
/// <summary>
/// Function to get bimodal settings.
/// </summary>
/// <returns>true if bimodal used</returns>
bool SCPSection6::getBimodal()
{
    return _Bimodal == 1;
}
/// <summary>
/// Function to set bimodal settings.
/// </summary>
/// <param name="bimodal">true if bimodal used</param>
void SCPSection6::setBimodal(bool bimodal)
{
    _Bimodal = (byte) (bimodal ? 1 : 0);
}

}
