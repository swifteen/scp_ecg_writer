#include "SCPSection2.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion.SCP
{
/// <summary>
/// Class that contains a SCP Huffman struct
/// </summary>
class SCPSection2::SCPHuffmanStruct
{
    SCPHuffmanStruct():Size(9)
    {}
    SCPHuffmanStruct(byte prefix, byte entire, byte tablemode, short value, uint code)
    {
        this.prefix = prefix;
        this.entire = entire;
        this.tablemode = tablemode;
        this.value = value;
        this.code = code;
    }

    /// <summary>
    /// Function to write SCP huffman struct.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(byte[] buffer, int offset)
    {
        if ((offset + Size) > buffer.Length)
        {
            return 0x1;
        }

        BytesTool::writeBytes(prefix, buffer, offset, sizeof(prefix), true);
        offset += sizeof(prefix);
        BytesTool::writeBytes(entire, buffer, offset, sizeof(entire), true);
        offset += sizeof(entire);
        BytesTool::writeBytes(tablemode, buffer, offset, sizeof(tablemode), true);
        offset += sizeof(tablemode);
        BytesTool::writeBytes(value, buffer, offset, sizeof(value), true);
        offset += sizeof(value);

        // Have to reverse the code, becaus SCP stores it that way.
        uint tempCode1 = code;
        uint tempCode2 = 0;
        for (int loper=prefix;loper > 0;loper--)
        {
            tempCode2 <<= 1;
            tempCode2 |= (tempCode1 & 0x1);
            tempCode1 >>= 1;
        }

        BytesTool::writeBytes((int)tempCode2, buffer, offset, sizeof(code), true);
        offset += sizeof(code);

        return 0x0;
    }
public:
    const int Size;

    byte prefix;
    byte entire;
    byte tablemode;
    short value;
    uint code;
};

// Defined in SCP.
ushort SCPSection2::_SectionID = 2;
ushort SCPSection2::_DefaultTable = 19999;
/// <summary>
/// Class contains section 2 (HuffmanTables).
/// </summary>
/// <remarks>
/// SCP uses a diffrent way to store codes, then UNIPRO. because I preffer the way UNIPRO
/// stores its codes. I store the SCP codes the same way as UNIPRO, but when I read/write
/// them from/to a buffer I reverse the code. This solution keeps the working of the SCP
/// and UNIPRO decode/encode very simalar, which is always a positive thing.
/// </remarks>
SCPSection2::SCPSection2()
{
    // currently selected table.
    _Selected = 0;

    // Part of the stored Data Structure.
    _NrTables = 0;
    _Tables = null;
}

int SCPSection2::_Write(byte[] buffer, int offset)
{
    BytesTool::writeBytes(_NrTables, buffer, offset, sizeof(_NrTables), true);
    offset += sizeof(_NrTables);
    if (_NrTables < _DefaultTable)
    {
        for (int table=0;table < _NrTables;table++)
        {
            BytesTool::writeBytes(_Tables[table].Length, buffer, offset, sizeof(_NrTables), true);
            offset += sizeof(_NrTables);
            for (int loper=0;loper < _Tables[table].Length;loper++)
            {
                int err = _Tables[table][loper].Write(buffer, offset);
                if (err != 0)
                {
                    return err << table;
                }
                offset += SCPHuffmanStruct.Size;
            }
        }
    }
    return 0x0;
}
void SCPSection2::_Empty()
{
    _Tables = null;
    _NrTables = 0;
}
int SCPSection2::_getLength()
{
    if (Works())
    {
        int sum = sizeof(_NrTables);
        if (_NrTables != _DefaultTable)
        {
            for (int table=0;table < _NrTables;table++)
            {
                sum += sizeof(_NrTables) + (_Tables[table].Length * SCPHuffmanStruct.Size);
            }
        }
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}
ushort SCPSection2::getSectionID()
{
    return _SectionID;
}
bool SCPSection2::Works()
{
    if ((_Tables != null)
            &&  (_NrTables == _Tables.Length))
    {
        for (int table=0;table < _Tables.Length;table++)
        {
            if (_Tables[table] == null)
            {
                return false;
            }
            for (int loper=0;loper < _Tables[table].Length;loper++)
            {
                if (_Tables[table][loper] == null)
                {
                    return false;
                }
            }
        }
        return true;
    }
    else if ((_Tables == null)
             &&	 ((_NrTables == _DefaultTable)
                  ||	  (_NrTables == 0)))
    {
        return true;
    }
    return false;
}
/// <summary>
/// Find next hit in byte array starting at an offset in bits.
/// </summary>
/// <param name="buffer">byte array containing encoded data</param>
/// <param name="offset">position (in bits) to start searching for a hit</param>
/// <returns>Info of hit.</returns>
SCPHuffmanStruct SCPSection2::InterpettingData(byte[] buffer, int offset)
{
    if ((_Tables[_Selected] != null)
            &&  (buffer != null))
    {
        uint bitBuffer = 0;
        int readMax = _Tables[_Selected][_Tables[_Selected].Length-1].prefix;
        for (int read=0;read < readMax&&((read + offset) >> 3) < buffer.Length;read++)
        {
            bitBuffer <<= 1;
            bitBuffer |= (uint) ((buffer[(offset + read) >> 3] >> (0x7 - ((offset + read) & 0x7))) & 0x1);

            for (int table = 0;table < _Tables[_Selected].Length;table++)
            {
                if ((bitBuffer == _Tables[_Selected][table].code)
                        &&  ((read + 1) == _Tables[_Selected][table].prefix))
                {
                    return _Tables[_Selected][table];
                }
            }
        }
    }
    return null;
}
/// <summary>
/// Function to encode data.
/// </summary>
/// <param name="data">signal to read from</param>
/// <param name="time">number of samples to use</param>
/// <param name="usedTable">table to use for encoding</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>byte array containing encoded data</returns>
byte[] SCPSection2::Encode(short[] data, int time, short usedTable, byte difference)
{
    if (Works() || _NrTables == 0)
    {
        if (_NrTables == _DefaultTable)
        {
            return InhouseEncode(data, time, difference);
        }
        else if (_NrTables == 0)
        {
            return NoEncode(data, time, difference);
        }
        else
        {
            return HuffmanTableEncode(data, time, usedTable, difference);
        }
    }
    return null;
}
/// <summary>
/// Function to encode signal using the default huffman table (using optimized code).
/// </summary>
/// <param name="data">signal to read from</param>
/// <param name="time">number of samples to use</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>byte array containing encoded data</returns>
static byte[] SCPSection2::InhouseEncode(short[] data, int time, byte difference)
{
    byte[] ret = null;

    // Check if input makes sense
    if ((data != null)
            &&	(time <= data.Length))
    {
        // Initialize some handy variables
        int currentBit = 0;

        // Make buffer for worst case.
        byte[] buffer = new byte[((time * 26) >> 3) + 1];

        // For each sample do encode.
        for (int currentTime=0;currentTime < time;currentTime++)
        {
            short code = 0;

            // Encode Differences.
            switch (difference)
            {
            case 0:
                code = data[currentTime];
                break;
            case 1:
                code = (short) ((currentTime < 1) ? data[currentTime] : data[currentTime] - data[currentTime - 1]);
                break;
            case 2:
                code = (short) ((currentTime < 2) ? data[currentTime] : data[currentTime] - (data[currentTime - 1] << 1) + data[currentTime - 2]);
                break;
            default:
                // Undefined difference used exit empty.
                return null;
            }

            // Do inhouse encode
            //System.Diagnostics.Debug.WriteLine("Begin inhouse!"+code.ToString());
            if ( code == -32768) code = 32767;
            if (Math.Abs(code) <= 8)
            {
                //  System.Diagnostics.Debug.WriteLine("Begin !");
                // if code 0 then add one 0 bit.
                if (code == 0)
                {
                    buffer[currentBit >> 3] <<= 1;
                    currentBit++;
                }
                else
                {
                    // add absolute number of 1 bits.
                    int codeAbs = Math.Abs(code);
                    for (int loper=0;loper < codeAbs;loper++)
                    {
                        buffer[currentBit >> 3] <<= 1;
                        buffer[currentBit >> 3] |= 1;
                        currentBit++;
                    }

                    // add one 0 bit.
                    buffer[currentBit >> 3] <<= 1;
                    currentBit++;

                    // add one more bit for positive of negative
                    buffer[currentBit >> 3] <<= 1;
                    if (code < 0)
                    {
                        buffer[currentBit >> 3] |= 1;
                    }
                    currentBit++;
                }
            }
            else
            {
                // Code doesn't fit in normal table do special.
                // First add nine 1 bits.
                for (int loper=0;loper < 9;loper++)
                {
                    buffer[currentBit >> 3] <<= 1;
                    buffer[currentBit >> 3] |= 1;
                    currentBit++;
                }

                // Add one more bit depending on size of code
                buffer[currentBit >> 3] <<= 1;
                int extraLength = 8;
                if (!((code <= 127) && (code >= -128)))
                {
                    buffer[currentBit >> 3] |= 1;
                    extraLength = 16;
                }
                currentBit++;

                // Add bits for extra code.
                for (extraLength--;extraLength >= 0;extraLength--)
                {
                    buffer[currentBit >> 3] <<= 1;
                    buffer[currentBit >> 3] |= (byte)((code >> extraLength) & 0x1);
                    currentBit++;
                    int currentBitShift = currentBit >> 3;
                }
            }
        }

        // Shift end to right position.
        if ((currentBit & 0x7) != 0x0)
        {
            buffer[(currentBit >> 3)] <<= (0x8 - (currentBit & 0x7));
            currentBit += (0x8 - (currentBit & 0x7));
        }
        else
        {
            // seems to solve a small encoding bug.
            currentBit += 8;
        }

        // Allocate a fitting buffer
        ret = new byte[(currentBit >> 3)];

        // Copy worst case buffer in fitting buffer.
        for (int loper = 0;loper < ret.Length;loper++)
        {
            ret[loper] = buffer[loper];
        }
    }
    return ret;
}
/// <summary>
/// Function to encode signal using the huffman table.
/// </summary>
/// <param name="data">signal to read from</param>
/// <param name="time">number of samples to use</param>
/// <param name="quanta">sample distance in signal</param>
/// <param name="usedTable">table to use for encoding</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>byte array containing encoded data</returns>
byte[] SCPSection2::HuffmanTableEncode(short[] data, int time, short usedTable, byte difference)
{
    byte[] ret = null;

    // Check if input makes sense
    if ((data != null)
            &&	(time <= data.Length))
    {
        // Initialize some handy variables
        int currentBit = 0;

        // Make buffer for worst case.
        byte[] buffer = null;

        if ((usedTable >= 0)
                &&  (usedTable < _Tables.Length)
                &&  (usedTable != _Selected))
        {
            uint code = 0;
            int len = 0;

            // get TableSwap position in HuffmanTable.
            int p = getTableSwap(usedTable);

            // Check if table swap is possible in this table.
            if (p >= 0)
            {
                // Store needed data from swap HuffmanStruct.
                code = _Tables[_Selected][p].code;
                len = _Tables[_Selected][p].entire;

                // set currently selected table.
                _Selected = usedTable;
            }

            // allocate buffer for worstcase.
            buffer = new byte[((len + (time * getWorstCase())) >> 3) + 1];

            // add table swap.
            for (len--;len >= 0;len--)
            {
                buffer[currentBit >> 3] <<= 1;
                buffer[currentBit >> 3] |= (byte)((code >> len) & 0x1);
                currentBit++;
            }
        }
        else
        {
            // No tables swap, so only space needed for worst case.
            buffer = new byte[((time * getWorstCase()) >> 3) + 1];
        }

        // For each sample do encode.
        for (int currentTime=0;currentTime < time;currentTime++)
        {
            short code = 0;

            // Encode Differences.
            switch (difference)
            {
            case 0:
                code = data[currentTime];
                break;
            case 1:
                code = (short) ((currentTime < 1) ? data[currentTime] : data[currentTime] - data[currentTime - 1]);
                break;
            case 2:
                code = (short) ((currentTime < 2) ? data[currentTime] : data[currentTime] - (data[currentTime - 1] << 1) + data[currentTime - 2]);
                break;
            default:
                // Undefined difference used exit empty.
                return null;
            }

            // Call Interpetting data to get an hit.
            SCPHuffmanStruct h = InterpettingData(code);
            if (h == null)
            {
                // not hit table or data must be wrong.
                return null;
            }

            // Push in the code.
            for (int loper=(h.prefix-1);loper >= 0;loper--)
            {
                buffer[currentBit >> 3] <<= 1;
                buffer[currentBit >> 3] |= (byte)((h.code >> loper) & 0x1);
                currentBit++;
            }

            // Push in the extra code, for special case.
            uint now = (uint) (code - h.value);
            for (int loper=(h.entire - h.prefix - 1);loper >= 0;loper--)
            {
                buffer[currentBit >> 3] <<= 1;
                buffer[currentBit >> 3] |= (byte)((code >> loper) & 0x1);
                currentBit++;
            }
        }

        // Shift end to right position.
        if ((currentBit & 0x7) != 0x0)
        {
            buffer[(currentBit >> 3)] <<= (0x8 - (currentBit & 0x7));
            currentBit += (0x8 - (currentBit & 0x7));
        }
        else
        {
            // seems to solve a small encoding bug.
            currentBit += 8;
        }

        // Allocate a fitting buffer
        ret = new byte[(currentBit >> 3)];

        // Copy worst case buffer in fitting buffer.
        for (int loper = 0;loper < ret.Length;loper++)
        {
            ret[loper] = buffer[loper];
        }
    }
    return ret;
}
/// <summary>
/// Function to find corresponding HuffmanStruct with value.
/// </summary>
/// <param name="value">value to search</param>
/// <returns>corresponding HuffmanStruct</returns>
SCPHuffmanStruct SCPSection2::InterpettingData(short value)
{
    // Check if selected Table exists
    if ((_Tables != null)
            &&	(_Tables[_Selected] != null))
    {
        // Search in structs of table.
        for (int loper=0;loper < _Tables[_Selected].Length;loper++)
        {
            SCPHuffmanStruct h = _Tables[_Selected][loper];
            // -1, because it can be positive and negative
            int extra = (h.entire - h.prefix - 1);

            // Check if value is equal to struct.
            if ((h.value == value)
                    &&  (h.tablemode != 0))
            {
                return h;
            }
            // Check if value fits in special case.
            else if ((extra > 0)
                     &&	 ((value - h.value) < (0x1 << extra))
                     &&   ((value - h.value) >= -(0x1 << extra))
                     &&	 (h.tablemode != 0))
            {
                return h;
            }
        }
    }
    return null;
}
/// <summary>
/// Function to store signal using no compression.
/// </summary>
/// <param name="data">signal to read from</param>
/// <param name="time">number of samples to use</param>
/// <param name="quanta">sample distance in signal</param>
/// <param name="difference">difference to use durring decoding</param>
/// <returns>byte array containing encoded data</returns>
static byte[] SCPSection2::NoEncode(short[] data, int time, byte difference)
{
    // Check if input data makes sense.
    if ((data != null)
            &&	(time <= data.Length))
    {
        // Initializing some handy variables
        int offset = 0;
        int sizeOfSample = sizeof(typeof(short));

        // Make buffer to contain samples
        byte[] ret = new byte[time * sizeOfSample];

        // For each sample do encode.
        for (int currentTime=0;currentTime < time;currentTime++)
        {
            short code = 0;

            // Encode Differences.
            switch (difference)
            {
            case 0:
                code = data[currentTime];
                break;
            case 1:
                code = (short) ((currentTime < 1) ? data[currentTime] : data[currentTime] - data[currentTime - 1]);
                break;
            case 2:
                code = (short) ((currentTime < 2) ? data[currentTime] : data[currentTime] - (data[currentTime - 1] << 1) + data[currentTime - 2]);
                break;
            default:
                // Undefined difference used exit empty.
                return null;
            }

            // Write data in buffer.
            BytesTool::writeBytes(code, ret, offset, sizeOfSample, true);
            offset += sizeOfSample;
        }
        return ret;
    }
    return null;
}
/// <summary>
/// Resets the current selected HuffmanTable
/// </summary>
void SCPSection2::ResetSelect()
{
    if (Works())
    {
        _Selected = 0;
    }
}
/// <summary>
/// Function to get position of table swap.
/// </summary>
/// <param name="table">prefered table</param>
/// <returns>position in current table</returns>
int SCPSection2::getTableSwap(int table)
{
    if (Works()
            &&  (table < _Tables.Length)
            &&	(_Selected < _Tables.Length))
    {
        for (int loper=0;loper < _Tables[_Selected].Length;loper++)
        {
            if (_Tables[_Selected][loper].tablemode == 0
                    &&  _Tables[_Selected][loper].value == (table + 1))
            {
                return loper;
            }
        }
    }
    return -1;
}
/// <summary>
/// Function to get binary length of worst case of selected table.
/// </summary>
/// <returns>length of worst case</returns>
int SCPSection2::getWorstCase()
{
    int worst = -1;
    if (Works())
    {
        if (this._NrTables == _DefaultTable)
        {
            worst = 26;
        }
        else if (_NrTables == 0)
        {
            worst = 16;
        }
        else
        {
            for (int loper=0;loper < _Tables[_Selected].Length;loper++)
            {
                if (_Tables[_Selected][loper].entire > worst)
                {
                    worst = _Tables[_Selected][loper].entire;
                }
            }
        }
    }
    return worst;
}
/// <summary>
/// Function to set standard huffman table.
/// </summary>
void SCPSection2::UseStandard()
{
    _NrTables = _DefaultTable;
}
/// <summary>
/// Function to set no huffman encoding.
/// </summary>
void SCPSection2::UseNoHuffman()
{
    _NrTables = 0;
}
}
