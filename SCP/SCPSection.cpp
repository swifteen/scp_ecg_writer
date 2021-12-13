#include "SCPSection.h"
#include "SCPFormat.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion
{
namespace SCP
{
int SCPSection::Size = 16;
/// <summary>
/// Constructor for a SCP Section.
/// </summary>
SCPSection::SCPSection()
{
	_Encoding = "ASCII";
    SectionID = 0;
	memset(Reserved,0,sizeof(Reserved));
    SectionID = getSectionID();	
//    Empty();//Usually you get this error when call your virtual function from constructor or destructor.
}

/// <summary>
/// Set encoding used for section.
/// </summary>
/// <param name="enc">encoding to use in section.</param>
void SCPSection::SetEncoding(const string& encoding)
{
    _Encoding = encoding;
}

#if 0
/// <summary>
/// Function to write an SCP Section.
/// </summary>
/// <param name="buffer">buffer allocated to write in</param>
/// <returns>error:
/// 0x00) succes
/// 0x01) section incorrect
/// 0x02) no buffer provided or buffer to small for header
/// rest) Section specific error </returns>
int SCPSection::Write(out byte[] buffer)
{
    buffer = null;
    if (Works())
    {
        buffer = new byte[_getLength() + Size];

        if (buffer.Length <= Size)
        {
            buffer = null;

            return 0;
        }

        int err = Write(buffer, 0);
        if (err != 0)
        {
            buffer = null;
        }
        return err;
    }
    return 0x1;
}
#endif
/// <summary>
/// Function to write an SCP Section.
/// </summary>
/// <param name="buffer">buffer to write to</param>
/// <param name="offset">position on buffer to start writing</param>
/// <returns>error:
/// 0x00) succes
/// 0x01) section incorrect
/// 0x02) no buffer provided or buffer to small for header
/// rest) Section specific error </returns>
int SCPSection::Write(uchar* buffer, int bufferLength,int offset)
{
    Length = _getLength() + Size;

    if (Length == Size)
        return 0;

    if (Works())
    {
        if ((buffer != null)
                &&	((offset + Length) <= bufferLength))
        {
            int crcoffset = offset;
            offset += sizeof(CRC);
            SectionID = getSectionID();
            BytesTool::writeBytes(SectionID, buffer, bufferLength,offset, sizeof(SectionID), true);
            offset += sizeof(SectionID);
            BytesTool::writeBytes(Length, buffer, bufferLength,offset, sizeof(Length), true);
            offset += sizeof(Length);
            BytesTool::writeBytes(SectionVersionNr, buffer, bufferLength,offset, sizeof(SectionVersionNr), true);
            offset += sizeof(SectionVersionNr);
            BytesTool::writeBytes(ProtocolVersionNr, buffer, bufferLength,offset, sizeof(ProtocolVersionNr), true);
            offset += sizeof(ProtocolVersionNr);
            offset += BytesTool::copy(buffer, bufferLength,offset, Reserved, kReservedLength,0, kReservedLength);

            int err = _Write(buffer, bufferLength,offset);
            if (err == 0)
            {
                CRCTool crc;
                crc.Init(CRCTool::CRC_CCITT);
                CRC = crc.CalcCRCITT(buffer, bufferLength,crcoffset + sizeof(CRC), Length - sizeof(CRC));
                BytesTool::writeBytes(CRC, buffer, bufferLength,crcoffset, sizeof(CRC), true);
            }
            return err << 2;
        }
        return 0x2;
    }
    return 0x1;
}
/// <summary>
/// Function to empty a SCP section.
/// </summary>
void SCPSection::Empty()
{
    CRC = 0;
    SectionID = getSectionID();
    Length = 0;
    SectionVersionNr = SCPFormat::DefaultSectionVersion;
    ProtocolVersionNr = SCPFormat::DefaultProtocolVersion;
    Reserved[0] = 0; Reserved[1] = 0; Reserved[2] = 0;
    Reserved[3] = 0; Reserved[4] = 0; Reserved[5] = 0;
    _Empty();
}
/// <summary>
/// Function to get the length of a SCP section.
/// </summary>
/// <returns>length of section</returns>
int SCPSection::getLength()
{
    int len = _getLength();
    return (len == 0 ? 0 : len + Size);
}
}
}
