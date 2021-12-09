#ifndef _CRCTOOL_H_
#define _CRCTOOL_H_
#include "ScpGlobal.h"

namespace Communication_IO_Tools
{
/// <summary>
/// Tool to calculate and add CRC codes to a string
/// </summary>
class CRCTool
{
    // Enumeration used in the init function to specify which CRC algorithm to use
public:
    enum CRCCode{CRC_CCITT, CRC16, CRC32};
public:
    CRCTool();
	~CRCTool();
    void Init(CRCCode CodingType);

    /// <summary>
    /// CalcCRCITT is an algorithm found on the web for calculating the CRCITT checksum
    /// It is included to demonstrate that although it looks different it is the same
    /// routine as the crcbitbybit* functions. But it is optimized and preconfigured for CRCITT.
    /// </summary>
    ushort CalcCRCITT(uchar* p, int pLength, int offset, int length);
private:
    //region subroutines
    ulong reflect (ulong crc, int bitnum);
    void generate_crc_table();
private:
    // 'order' [1..32] is the CRC polynom order, counted without the leading '1' bit
    // 'polynom' is the CRC polynom without leading '1' bit
    // 'direct' [0,1] specifies the kind of algorithm: 1=direct, no augmented zero bits
    // 'crcinit' is the initial CRC value belonging to that algorithm
    // 'crcxor' is the final XOR value
    // 'refin' [0,1] specifies if a data byte is reflected before processing (UART) or not
    // 'refout' [0,1] specifies if the CRC will be reflected before XOR
    // Data character string
    // For CRC-CCITT : order = 16, direct=1, poly=0x1021, CRCinit = 0xFFFF, crcxor=0; refin =0, refout=0
    // For CRC16:	   order = 16, direct=1, poly=0x8005, CRCinit = 0x0, crcxor=0x0; refin =1, refout=1
    // For CRC32:	   order = 32, direct=1, poly=0x4c11db7, CRCinit = 0xFFFFFFFF, crcxor=0xFFFFFFFF; refin =1, refout=1
    // Default : CRC-CCITT

    int   order;
    ulong polynom;
    int   direct;
    ulong crcinit;
    ulong crcxor;
    int   refin;
    int   refout;

    ulong crcmask;
    ulong crchighbit;
    ulong crcinit_direct;
    ulong crcinit_nondirect;
    ulong* crctab;
};
}

#endif  /*#ifndef _CRCTOOL_H_*/
