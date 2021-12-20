#include "SCPSection6.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{


/// <summary>
/// Class contains section 6 (Rhythm data section).
/// </summary>

ushort SCPSection6::_SectionID = 6;


SCPSection6::SCPSection6()
{
    SCPSection::Empty();
    // special variable for setting nr leads before a read.
    _NrLeads = 0;
    // Part of the stored Data Structure.
    _AVM = 0;
    _TimeInterval = 0;
    _Difference = 0;
    _Bimodal = 0;
    _DataLength.clear();
    _DataRealLength.clear();
    _Data.clear();
    int size = _Data.size();

    for (int i = 0; i < size; i++) {
        _Data[i] = null;
    }
}

SCPSection6::~SCPSection6()
{
    int size = _Data.size();

    for (int i = 0; i < size; i++) {
        if (_Data[i] != null) {
            delete [] _Data[i];
            _Data[i] = null;
        }
    }
}

int SCPSection6::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_AVM, buffer, bufferLength, offset, sizeof(_AVM), true);
    offset += sizeof(_AVM);
    BytesTool::writeBytes(_TimeInterval, buffer, bufferLength, offset, sizeof(_TimeInterval), true);
    offset += sizeof(_TimeInterval);
    BytesTool::writeBytes(_Difference, buffer, bufferLength, offset, sizeof(_Difference), true);
    offset += sizeof(_Difference);
    BytesTool::writeBytes(_Bimodal, buffer, bufferLength, offset, sizeof(_Bimodal), true);
    offset += sizeof(_Bimodal);
    int offset2 = offset + (_Data.size() * sizeof(_DataLength[0]));

    for (int loper = 0; loper < _Data.size(); loper++) {
        BytesTool::writeBytes(_DataLength[loper], buffer, bufferLength, offset, sizeof(_DataLength[loper]), true);
        offset += sizeof(_DataLength[loper]);
        BytesTool::copy(buffer, bufferLength, offset2, _Data[loper], _DataRealLength[loper], 0, _DataRealLength[loper]);
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
    _DataLength.clear();
    _Data.clear();
    _DataRealLength.clear();
}
int SCPSection6::_getLength()
{
    if (Works()) {
        int sum = sizeof(_AVM) + sizeof(_TimeInterval) + sizeof(_Difference) + sizeof(_Bimodal);
        sum += (_Data.size() * sizeof(_DataLength[0]));

        for (int loper = 0; loper < _Data.size(); loper++) {
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
    if ((_Data.size() > 0)
        && (_DataLength.size() > 0)
        && (_Data.size() == _DataLength.size())) {
        for (int loper = 0; loper < _Data.size(); loper++) {
            if ((_Data[loper] == null)
                || (_DataLength[loper] < _DataRealLength[loper])) {
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
int SCPSection6::EncodeData(std::vector<short*>& data,
                            std::vector<int>& dataLength,
                            SCPSection2* tables,
                            SCPSection3* leadDefinition,
                            SCPSection4* qrsLocations,
                            int medianFreq,
                            uchar difference)
{
    int localFreq = getSamplesPerSecond();

    if ((data.size() > 0)
        && (tables != null)
        && (leadDefinition != null)
        && (localFreq > 0)) {
        if ((medianFreq <= 0)
            || (medianFreq == localFreq)) {
            medianFreq = 1;
            localFreq = 1;
        }

        if ((_Bimodal == 0x1)
            && (qrsLocations == null)) {
            return 2;
        }

        ushort nrleads = leadDefinition->getNrLeads();

        if ((nrleads != data.size()) || (nrleads != dataLength.size())) {
            return 8;
        }

        _Data.resize(nrleads);
        _DataLength.resize(nrleads);
        _DataRealLength.resize(nrleads);

        for (int loper = 0; loper < nrleads; loper++) {
            if (data[loper] == null) {
                return 4;
            }

            short* temp = data[loper];
            int dataSingleLength = dataLength[loper];
            int time = (leadDefinition->getLeadLength(loper) * localFreq) / medianFreq;
#if 0

            if (localFreq != medianFreq) {
                int rate = (medianFreq / localFreq);

                // Bimodal part might be buggy unable to test.
                if ((_Bimodal == 0x1)
                    && ((medianFreq % localFreq) == 0)
                    && (rate > 0)
                    && (rate < 5)) {
                    // Calculate nr of samples stored in section.
                    time = 0;
                    int nrzones = qrsLocations.getNrProtectedZones();

                    for (int zone = 0; zone < nrzones; zone++) {
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
                    int time1 = 0;
                    int time2 = 0;

                    while ((time1 < temp.Length)
                           && (time2 <= leadLength)
                           && (time2 < data[loper].Length)) {
                        int zone = 0;
                        int end = qrsLocations.getNrProtectedZones();

                        for (; zone < end; zone++) {
                            if ((qrsLocations.getProtectedLength(zone) > 0)
                                && (time2 + time2Offset >= qrsLocations.getProtectedStart(zone))
                                && (time2 + time2Offset <= qrsLocations.getProtectedEnd(zone))) {
                                break;
                            }
                        }

                        if (zone < end) {
                            temp[time1] = data[loper][time2++];
                        }
                        else {
                            int Sum = 0;

                            for (int sumLoper = 0; sumLoper < rate; sumLoper++) {
                                Sum += data[loper][time2 + sumLoper];
                            }

                            temp[time1] = (short)(Sum / rate);
                            time2 += rate;
                        }

                        time1++;
                    }
                }
                else {
                    _Bimodal = 0;
                    ECGTool.ResampleLead(temp, medianFreq, localFreq, out temp);
                }
            }

#endif
            _Difference = difference;
            int encodeLength = 0;
            _Data[loper] = tables->Encode(temp, dataSingleLength, time, 0, _Difference, &encodeLength);

            if ((_Data[loper] == null) || (0 == encodeLength)) {
                _Data.clear();
                _DataLength.clear();
                return 8;
            }

            _DataLength[loper] = (ushort) encodeLength;
            _DataRealLength[loper] = (ushort) encodeLength;

            if ((_DataLength[loper] & 0x1) == 0x1) {
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
    if (_AVM > 0) {
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
    if (avm > 0) {
        _AVM  = (ushort)(avm * 1000);
    }
}
/// <summary>
/// Function to get samples per second used in data.
/// </summary>
/// <returns>samples per second</returns>
int SCPSection6::getSamplesPerSecond()
{
    if (_TimeInterval > 0) {
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
    if (sps > 0) {
        _TimeInterval = (ushort)(1000000 / sps);
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
    _Bimodal = (ushort)(bimodal ? 1 : 0);
}
}
}
