#include "SCPSection.h"
#include "SCPFormat.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;

namespace ECGConversion.SCP
{
int SCPSection::Size = 16;
int SCPSection::_ReservedLength = 6;
/// <summary>
/// Constructor for a SCP Section.
/// </summary>
SCPSection::SCPSection()
{
    _Encoding = System.Text.Encoding.Default;
    SectionID = 0;
    Reserved = new byte[_ReservedLength];
    SectionID = getSectionID();
    Empty();
}
/// <summary>
/// Set encoding used for section.
/// </summary>
/// <param name="enc">encoding to use in section.</param>
void SCPSection::SetEncoding(System.Text.Encoding enc)
{
    _Encoding = enc;
}

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
int SCPSection::Write(byte[] buffer, int offset)
{
    Length = _getLength() + Size;

    if (Length == Size)
        return 0;

    if (Works())
    {
        if ((buffer != null)
                &&	((offset + Length) <= buffer.Length))
        {
            int crcoffset = offset;
            offset += sizeof(CRC);
            SectionID = getSectionID();
            BytesTool::writeBytes(SectionID, buffer, offset, sizeof(SectionID), true);
            offset += sizeof(SectionID);
            BytesTool::writeBytes(Length, buffer, offset, sizeof(Length), true);
            offset += sizeof(Length);
            BytesTool::writeBytes(SectionVersionNr, buffer, offset, sizeof(SectionVersionNr), true);
            offset += sizeof(SectionVersionNr);
            BytesTool::writeBytes(ProtocolVersionNr, buffer, offset, sizeof(ProtocolVersionNr), true);
            offset += sizeof(ProtocolVersionNr);
            offset += BytesTool::copy(buffer, offset, Reserved, 0, _ReservedLength);

            int err = _Write(buffer, offset);
            if (err == 0)
            {
                CRCTool crc;
                crc.Init(CRCTool::CRCCode::CRC_CCITT);
                CRC = crc.CalcCRCITT(buffer, crcoffset + sizeof(CRC), Length - sizeof(CRC));
                BytesTool::writeBytes(CRC, buffer, crcoffset, sizeof(CRC), true);
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
