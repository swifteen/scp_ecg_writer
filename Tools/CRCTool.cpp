#include "CRCTool.h"

namespace Communication_IO_Tools
{
/// <summary>
/// Tool to calculate and add CRC codes to a string
/// </summary>
CRCTool::CRCTool()
{
    // 'order' [1..32] is the CRC polynom order, counted without the leading '1' bit
    // 'polynom' is the CRC polynom without leading '1' bit
    // 'direct' [0,1] specifies the kind of algorithm: 1=direct, no augmented zero bits
    // 'crcinit' is the initial CRC value belonging to that algorithm
    // 'crcxor' is the final XOR value
    // 'refin' [0,1] specifies if a data byte is reflected before processing (UART) or not
    // 'refout' [0,1] specifies if the CRC will be reflected before XOR
    // Data character string
    // For CRC-CCITT : order = 16, direct=1, poly=0x1021, CRCinit = 0xFFFF, crcxor=0; refin =0, refout=0
    // For CRC16:      order = 16, direct=1, poly=0x8005, CRCinit = 0x0, crcxor=0x0; refin =1, refout=1
    // For CRC32:      order = 32, direct=1, poly=0x4c11db7, CRCinit = 0xFFFFFFFF, crcxor=0xFFFFFFFF; refin =1, refout=1
    // Default : CRC-CCITT

    order      = 16;
    polynom    = 0x1021;
    direct     = 1;
    crcinit    = 0xFFFF;
    crcxor     = 0x0;
    refin      = 0;
    refout     = 0;

    crcmask;
    crchighbit;
    crcinit_direct;
    crcinit_nondirect;
    crctab = new ulong[256];
}

CRCTool::~CRCTool()
{
	if(crctab != null)
	{
		delete [] crctab;
		crctab = null;
	}
}

void CRCTool::Init(CRCCode CodingType)
{
    switch( CodingType )
    {
    case CRC_CCITT:
        order = 16; direct=1; polynom=0x1021; crcinit = 0xFFFF; crcxor=0; refin =0; refout=0;
        break;
    case CRC16:
        order = 16; direct=1; polynom=0x8005; crcinit = 0x0; crcxor=0x0; refin =1; refout=1;
        break;
    case CRC32:
        order = 32; direct=1; polynom=0x4c11db7; crcinit = 0xFFFFFFFF; crcxor=0xFFFFFFFF; refin =1; refout=1;
        break;
    }

    // Initialize all variables for seeding and builing based upon the given coding type
    // at first, compute constant bit masks for whole CRC and CRC high bit

    crcmask = ((((ulong)1<<(order-1))-1)<<1)|1;
    crchighbit = (ulong)1<<(order-1);

    // generate lookup table
    generate_crc_table();

    ulong bit, crc;
    int i;
    if ( direct == 0 )
    {
        crcinit_nondirect = crcinit;
        crc = crcinit;
        for (i=0; i<order; i++)
        {
            bit = crc & crchighbit;
            crc<<= 1;
            if ( bit != 0 )
            {
                crc^= polynom;
            }
        }
        crc&= crcmask;
        crcinit_direct = crc;
    }
    else
    {
        crcinit_direct = crcinit;
        crc = crcinit;
        for (i=0; i<order; i++)
        {
            bit = crc & 1;
            if (bit != 0)
            {
                crc^= polynom;
            }
            crc >>= 1;
            if (bit != 0)
            {
                crc|= crchighbit;
            }
        }
        crcinit_nondirect = crc;
    }
}
/// <summary>
/// CalcCRCITT is an algorithm found on the web for calculating the CRCITT checksum
/// It is included to demonstrate that although it looks different it is the same
/// routine as the crcbitbybit* functions. But it is optimized and preconfigured for CRCITT.
/// </summary>
ushort CRCTool::CalcCRCITT(uchar* p, int pLength,int offset, int length)
{
    // Works like CalcCRCITT(byte[] p), except you now calculate a CRC for a specific part of the byte array.
    int iBufferIndex = 0;
    uint uiCRCITTSum = 0xFFFF;
    uint uiByteValue;

    if (offset > 0)
    {
        iBufferIndex = offset;
        length += offset;
    }
    if (length > pLength)
        length = pLength;

    for (; iBufferIndex < length; iBufferIndex++)
    {
        uiByteValue = ( (uint) p[iBufferIndex] << 8);
        for ( int iBitIndex = 0; iBitIndex < 8; iBitIndex++ )
        {
            if ( ( (uiCRCITTSum^uiByteValue) & 0x8000) != 0 )
            {
                uiCRCITTSum = (uiCRCITTSum <<1 ) ^ 0x1021;
            }
            else
            {
                uiCRCITTSum <<= 1;
            }
            uiByteValue <<=1;
        }
    }
    return (ushort)uiCRCITTSum;
}

//region subroutines
ulong CRCTool::reflect (ulong crc, int bitnum)
{
    // reflects the lower 'bitnum' bits of 'crc'

    ulong i, j=1, crcout = 0;

    for ( i = (ulong)1 <<(bitnum-1); i != 0; i>>=1)
    {
        if ( ( crc & i ) != 0 )
        {
            crcout |= j;
        }
        j<<= 1;
    }
    return (crcout);
}

void CRCTool::generate_crc_table()
{

    // make CRC lookup table used by table algorithms

    int i, j;
    ulong bit, crc;

    for (i=0; i<256; i++)
    {
        crc=(ulong)i;
        if ( refin !=0 )
        {
            crc=reflect(crc, 8);
        }
        crc<<= order-8;

        for (j=0; j<8; j++)
        {
            bit = crc & crchighbit;
            crc<<= 1;
            if ( bit !=0 ) crc^= polynom;
        }

        if (refin != 0)
        {
            crc = reflect(crc, order);
        }
        crc&= crcmask;
        crctab[i]= crc;
    }
}
}
