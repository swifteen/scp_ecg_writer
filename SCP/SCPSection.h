#ifndef _SCPSECTION_H_
#define _SCPSECTION_H_
#include "ScpGlobal.h"

namespace ECGConversion.SCP
{
/// <summary>
/// abstract class describing basic form of a section.
/// </summary>
class SCPSection
{
public:
    /// <summary>
    /// Constructor for a SCP Section.
    /// </summary>
    SCPSection();
    /// <summary>
    /// Set encoding used for section.
    /// </summary>
    /// <param name="enc">encoding to use in section.</param>
    void SetEncoding(System.Text.Encoding enc);
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
    int Write(byte[] buffer, int offset);
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
    ushort getSectionID() = 0;
    /// <summary>
    /// Function to check working of section.
    /// </summary>
    /// <returns>true: working
    /// false not working</returns>
    virtual bool Works() = 0;
protected:
    /// <summary>
    /// Hidden read function is called by Read().
    /// </summary>
    /// <param name="buffer">byte array to read from</param>
    /// <param name="offset">position to start reading</param>
    /// <returns>0 on success</returns>
    virtual int _Read(byte[] buffer, int offset) = 0;
    /// <summary>
    /// Hidden write function is called by Write().
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    virtual int _Write(byte[] buffer, int offset) = 0;
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
    // encoding of scp file
    System.Text.Encoding _Encoding;

    // Content of Header of section.
    ushort CRC;
    ushort SectionID;
    int Length;
    byte SectionVersionNr;
    byte ProtocolVersionNr;
    byte[] Reserved;
private:
    static int _ReservedLength;
};

}

#endif  /*#ifndef _SCPSECTION_H_*/
