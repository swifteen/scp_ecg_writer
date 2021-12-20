#ifndef _BYTESTOOL_H_
#define _BYTESTOOL_H_
#include "ScpGlobal.h"

namespace Communication_IO_Tools
{
class BytesTool
{
public:
    /// <summary>
    /// Function to write an integer to a buffer at an offset.
    /// </summary>
    /// <param name="values">integer to write</param>
    /// <param name="buffer">buffer to write to</param>
    /// <param name="offset">position to start writing</param>
    /// <param name="bytes">nr bytes to write</param>
    /// <param name="littleEndian">little endian or big endian integer</param>
    /// <returns></returns>
    static bool writeBytes(long values, uchar* buffer, int bufferLength, int offset, int bytes, bool littleEndian);

    /// <summary>
    /// Function to write a string too a byte array at a given offset
    /// </summary>
    /// <param name="src">to read from</param>
    /// <param name="buffer">to write the string too</param>
    /// <param name="offset">position to start reading</param>
    /// <param name="length">max length of string</param>
    static void writeString(const std::string& src, uchar* buffer, int bufferLength, int offset, int length);

    /// <summary>
    /// Function to write a string too a byte array at a given offset
    /// </summary>
    /// <param name="enc">enconding type</param>
    /// <param name="src">to read from</param>
    /// <param name="buffer">to write the string too</param>
    /// <param name="offset">position to start reading</param>
    /// <param name="length">max length of string</param>
    static void writeString(const std::string& dstEncoding,
                            const std::string& src,
                            uchar* buffer,
                            int bufferLength,
                            int offset,
                            int length);
    static int convert_charset(const char* from_charset,
                               const char* to_charset,
                               char* src_buf,
                               size_t src_len,
                               char* dst_buf,
                               size_t* dst_len);
    /// <summary>
    /// Function to copy content of one buffer to another.
    /// </summary>
    /// <param name="dst">destination buffer</param>
    /// <param name="offdst">offset in destination buffer</param>
    /// <param name="src">source buffer</param>
    /// <param name="offsrc">offset in source buffer</param>
    /// <param name="length">number bytes to copy</param>
    static int copy(uchar* dst, int dstLength, int offdst, const uchar* src, int srcLength, int offsrc, int length);

    /// <summary>
    /// Function to empty a buffer to a defined number.
    /// </summary>
    /// <param name="buffer">destination buffer</param>
    /// <param name="offset">offset in buffer</param>
    /// <param name="nrbytes">number byte to empty</param>
    /// <param name="type">number to empty to</param>
    static void emptyBuffer(uchar* buffer, int bufferLength, int offset, int nrbytes, uchar type);
};
}


#endif  /*#ifndef _BYTESTOOL_H_*/
