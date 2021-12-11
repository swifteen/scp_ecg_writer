#ifndef _SCPSECTION2_H_
#define _SCPSECTION2_H_
#include "SCPSection.h"

namespace ECGConversion
{
namespace SCP
{
class SCPHuffmanStruct;

/// <summary>
/// Class contains section 2 (HuffmanTables).
/// </summary>
/// <remarks>
/// SCP uses a diffrent way to store codes, then UNIPRO. because I preffer the way UNIPRO
/// stores its codes. I store the SCP codes the same way as UNIPRO, but when I read/write
/// them from/to a buffer I reverse the code. This solution keeps the working of the SCP
/// and UNIPRO decode/encode very simalar, which is always a positive thing.
/// </remarks>
class SCPSection2 : public SCPSection
{
public:
	SCPSection2();
    ushort getSectionID();
    bool Works();

    /// <summary>
    /// Function to encode data.
    /// </summary>
    /// <param name="data">signal to read from</param>
    /// <param name="time">number of samples to use</param>
    /// <param name="usedTable">table to use for encoding</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>byte array containing encoded data</returns>
	uchar* Encode(short* data, int dataLength,int time, short usedTable, uchar difference,int* encodeLength);
    /// <summary>
    /// Function to encode signal using the default huffman table (using optimized code).
    /// </summary>
    /// <param name="data">signal to read from</param>
    /// <param name="time">number of samples to use</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>byte array containing encoded data</returns>
	static uchar* InhouseEncode(short* data, int dataLength,int time, uchar difference,int* encodeLength);
#if 0
    /// <summary>
    /// Function to store signal using no compression.
    /// </summary>
    /// <param name="data">signal to read from</param>
    /// <param name="time">number of samples to use</param>
    /// <param name="quanta">sample distance in signal</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>byte array containing encoded data</returns>
    static byte[] NoEncode(short[] data, int time, uchar difference);
#endif

    /// <summary>
    /// Resets the current selected HuffmanTable
    /// </summary>
    void ResetSelect();

    /// <summary>
    /// Function to set standard huffman table.
    /// </summary>
    void UseStandard();

    /// <summary>
    /// Function to set no huffman encoding.
    /// </summary>
    void UseNoHuffman();

protected:
    int _Write(uchar* buffer, int bufferLength, int offset);
    void _Empty();
    int _getLength();
#if 0
    /// <summary>
    /// Find next hit in byte array starting at an offset in bits.
    /// </summary>
    /// <param name="buffer">byte array containing encoded data</param>
    /// <param name="offset">position (in bits) to start searching for a hit</param>
    /// <returns>Info of hit.</returns>
    SCPHuffmanStruct InterpettingData(byte[] buffer, int offset);

    /// <summary>
    /// Function to encode signal using the huffman table.
    /// </summary>
    /// <param name="data">signal to read from</param>
    /// <param name="time">number of samples to use</param>
    /// <param name="quanta">sample distance in signal</param>
    /// <param name="usedTable">table to use for encoding</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>byte array containing encoded data</returns>
    byte[] HuffmanTableEncode(short[] data, int time, short usedTable, uchar difference);

    /// <summary>
    /// Function to find corresponding HuffmanStruct with value.
    /// </summary>
    /// <param name="value">value to search</param>
    /// <returns>corresponding HuffmanStruct</returns>
    SCPHuffmanStruct InterpettingData(short value);

    /// <summary>
    /// Function to get position of table swap.
    /// </summary>
    /// <param name="table">prefered table</param>
    /// <returns>position in current table</returns>
    int getTableSwap(int table);
#endif

    /// <summary>
    /// Function to get binary length of worst case of selected table.
    /// </summary>
    /// <returns>length of worst case</returns>
    int getWorstCase();
private:
    // Defined in SCP.
    static ushort _SectionID;
    static ushort _DefaultTable;

    // currently selected table.
    int _Selected;

    // Part of the stored Data Structure.
    ushort _NrTables;
#if 0 //use DefaultTable
    SCPHuffmanStruct[][] _Tables;
#endif
};
}
}
#endif  /*#ifndef _SCPSECTION2_CPP_*/
