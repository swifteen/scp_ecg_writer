#include "SCPSection5.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;


namespace ECGConversion
{
namespace SCP
{
ushort SCPSection5::_SectionID = 5;

/// <summary>
/// Class contains section 5 (Reference beat data section).
/// </summary>

SCPSection5::SCPSection5()
{
    // special variable for setting nr leads before a read.
    _NrLeads = 0;
    // Part of the stored Data Structure.
    _AVM = 0;
    _TimeInterval = 0;
    _Difference = 0;
    _Reserved = 0;
    _Data.clear();
    _DataLength.clear();
    _DataRealLength.clear();
    int size = _Data.size();

    for (int i = 0; i < size; i++) {
        _Data[i] = null;
    }
}

SCPSection5::~SCPSection5()
{
    int size = _Data.size();

    for (int i = 0; i < size; i++) {
        if (_Data[i] != null) {
            delete [] _Data[i];
            _Data[i] = null;
        }
    }
}

int SCPSection5::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_AVM, buffer, bufferLength, offset, sizeof(_AVM), true);
    offset += sizeof(_AVM);
    BytesTool::writeBytes(_TimeInterval, buffer, bufferLength, offset, sizeof(_TimeInterval), true);
    offset += sizeof(_TimeInterval);
    BytesTool::writeBytes(_Difference, buffer, bufferLength, offset, sizeof(_Difference), true);
    offset += sizeof(_Difference);
    BytesTool::writeBytes(_Reserved, buffer, bufferLength, offset, sizeof(_Reserved), true);
    offset += sizeof(_Reserved);
    int offset2 = offset + (_Data.size() * sizeof(_DataLength[0]));

    for (int loper = 0; loper < _Data.size(); loper++) {
        BytesTool::writeBytes(_DataLength[loper],
                              buffer,
                              bufferLength,
                              offset,
                              sizeof(_DataLength[loper]),
                              true);
        offset += sizeof(_DataLength[loper]);
        BytesTool::copy(buffer,
                        bufferLength,
                        offset2,
                        _Data[loper],
                        _DataRealLength[loper],
                        0,
                        _DataRealLength[loper]);
        offset2 += _DataLength[loper];
    }

    return 0x00;
}
void SCPSection5::_Empty()
{
    _AVM = 0;
    _TimeInterval = 0;
    _Difference = 0;
    _Reserved = 0;
    _Data.clear();
    _DataLength.clear();
    _DataRealLength.clear();
}
int SCPSection5::_getLength()
{
    if (Works()) {
        int sum = sizeof(_AVM) + sizeof(_TimeInterval) + sizeof(_Difference) + sizeof(_Reserved);
        sum += (_Data.size() * sizeof(_DataLength[0]));

        for (int loper = 0; loper < _Data.size(); loper++) {
            sum += _DataLength[loper];
        }

        return ((sum % 2) == 0 ? sum : sum + 1);
    }

    return 0;
}
ushort SCPSection5::getSectionID()
{
    return _SectionID;
}
bool SCPSection5::Works()
{
    if ((_Data.size() == _DataLength.size())
        && (_Data.size() > 0)) {
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
void SCPSection5::setNrLeads(ushort nrleads)
{
    _NrLeads = nrleads;
}

#if 0 //current only support rhythm data
/// <summary>
/// Function to encode data in this section.
/// </summary>
/// <param name="data">Rhythm data to encode</param>
/// <param name="tables">Huffman table to use during enconding</param>
/// <param name="medianLength">contains length of median data in msec</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>0 on succes</returns>
int SCPSection5::EncodeData(short* dataArray,
                            int nrleads,
                            int dataSingleLength,
                            SCPSection2* tables,
                            ushort medianLength,
                            uchar difference)
{
    if ((tables != null)
        && (dataArray != null)) {
        _Data.resize(nrleads);
        _DataLength.resize(nrleads);
        _DataRealLength.resize(nrleads);

        for (int loper = 0; loper < nrleads; loper++) {
            if (dataArray + loper * dataSingleLength == null) {
                return 2;
            }

            _Difference = difference;
            int encodeLength = 0;
            _Data[loper] = tables->Encode(dataArray + loper * dataSingleLength,
                                          dataSingleLength,
                                          medianLength,
                                          0,
                                          _Difference,
                                          &encodeLength);

            if ((_Data[loper] == null) || (0 == encodeLength)) {
                _Data.clear();
                _DataLength.clear();
                return 4;
            }

            _DataLength[loper] = (ushort) encodeLength;

            if ((_DataLength[loper] & 0x1) == 0x1) {
                _DataLength[loper]++;
            }
        }

        return 0;
    }

    return 1;
}
#endif
/// <summary>
/// Function to get AVM.
/// </summary>
/// <returns>AVM in uV</returns>
double SCPSection5::getAVM()
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
void SCPSection5::setAVM(double avm)
{
    if (avm > 0) {
        _AVM  = (ushort)(avm * 1000);
    }
}
/// <summary>
/// Function to get samples per second used in data.
/// </summary>
/// <returns>samples per second</returns>
int SCPSection5::getSamplesPerSecond()
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
void SCPSection5::setSamplesPerSecond(int sps)
{
    if (sps > 0) {
        _TimeInterval = (ushort)(1000000 / sps);
    }
}
}
}
