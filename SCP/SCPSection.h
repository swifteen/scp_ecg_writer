#ifndef _SCPSECTION_H_
#define _SCPSECTION_H_
#include "ScpGlobal.h"

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// abstract class describing basic form of a section.
/// </summary>
class SCPSection
{
public:
	enum
	{
		kReservedLength = 6,
	};
    /// <summary>
    /// Constructor for a SCP Section.
    /// </summary>
    SCPSection();
#if 0//TODO
    /// <summary>
    /// Set encoding used for section.
    /// </summary>
    /// <param name="enc">encoding to use in section.</param>
    void SetEncoding(System.Text.Encoding enc);
#endif
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
    int Write(out byte[] buffer);
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
    int Write(uchar* buffer, int bufferLength, int offset);
    /// <summary>
    /// Function to empty a SCP section.
    /// </summary>
    void Empty();
    /// <summary>
    /// Function to get the length of a SCP section.
    /// </summary>
    /// <returns>length of section</returns>
    int getLength();
    /// <summary>
    /// Function to get section ID of section.
    /// </summary>
    /// <returns>section id</returns>
    virtual ushort getSectionID() {return 0;}
    /// <summary>
    /// Function to check working of section.
    /// </summary>
    /// <returns>true: working
    /// false not working</returns>
    virtual bool Works() = 0;
protected:
    /// <summary>
    /// Hidden write function is called by Write().
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    virtual int _Write(uchar* buffer, int bufferLength, int offset) = 0;
    /// <summary>
    /// Hidden empty function is called by Empty().
    /// </summary>
    virtual void _Empty() = 0;
    /// <summary>
    /// Hidden length function is called getLength().
    /// </summary>
    /// <returns></returns>
    virtual int _getLength() = 0;
public:
    static int Size;
protected:
#if 0//TODO
    // encoding of scp file
    System.Text.Encoding _Encoding;
#endif

    // Content of Header of section.
    ushort CRC;
    ushort SectionID;
    int Length;
    uchar SectionVersionNr;
    uchar ProtocolVersionNr;
    uchar Reserved[kReservedLength];
};

}
}
#endif  /*#ifndef _SCPSECTION_H_*/
