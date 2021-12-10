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
    _DataLength = null;
    _Data = null;
}

int SCPSection5::_Write(uchar* buffer, int bufferLength, int offset)
{
    BytesTool::writeBytes(_AVM, buffer, bufferLength,offset, sizeof(_AVM), true);
    offset += sizeof(_AVM);
    BytesTool::writeBytes(_TimeInterval, buffer, bufferLength,offset, sizeof(_TimeInterval), true);
    offset += sizeof(_TimeInterval);
    BytesTool::writeBytes(_Difference, buffer, bufferLength,offset, sizeof(_Difference), true);
    offset += sizeof(_Difference);
    BytesTool::writeBytes(_Reserved, buffer, bufferLength,offset, sizeof(_Reserved), true);
    offset += sizeof(_Reserved);

    int offset2 = offset + (_Data.Length * sizeof(_DataLength[0]));

    for (int loper=0;loper < _Data.Length;loper++)
    {
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
						_Data[loper].Length, 
						0, 
						_Data[loper].Length);
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
    _DataLength = null;
    _Data = null;
}
int SCPSection5::_getLength()
{
    if (Works())
    {
        int sum = sizeof(_AVM) + sizeof(_TimeInterval) + sizeof(_Difference) + sizeof(_Reserved);
        sum += (_Data.Length * sizeof(_DataLength[0]));
        for (int loper=0;loper < _Data.Length;loper++)
        {
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
    if ((_Data != null)
            &&  (_DataLength != null)
            &&  (_Data.Length == _DataLength.Length)
            &&  (_Data.Length > 0))
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
void SCPSection5::setNrLeads(ushort nrleads)
{
    _NrLeads = nrleads;
}

/// <summary>
/// Function to encode data in this section.
/// </summary>
/// <param name="data">Rhythm data to encode</param>
/// <param name="tables">Huffman table to use during enconding</param>
/// <param name="medianLength">contains length of median data in msec</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>0 on succes</returns>
int SCPSection5::EncodeData(short[][] data, SCPSection2 tables, ushort medianLength, uchar difference)
{
    if ((tables != null)
            &&  (data != null))
    {
        ushort nrleads = (ushort) data.Length;
        _Data = new byte[nrleads][];
        _DataLength = new ushort[nrleads];
        for (int loper=0;loper < nrleads;loper++)
        {
            if (data[loper] == null)
            {
                return 2;
            }

            _Difference = difference;
            _Data[loper] = tables.Encode(data[loper], medianLength, 0, _Difference);
            if (_Data[loper] == null)
            {
                _Data = null;
                _DataLength = null;
                return 4;
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
double SCPSection5::getAVM()
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
void SCPSection5::setAVM(double avm)
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
int SCPSection5::getSamplesPerSecond()
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
void SCPSection5::setSamplesPerSecond(int sps)
{
    if (sps > 0)
    {
        _TimeInterval = (ushort) (1000000 / sps);
    }
}
}
}
