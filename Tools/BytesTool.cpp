#include "BytesTool.h"

namespace Communication.IO.Tools
{
/// <summary>
/// Function to write an integer to a buffer at an offset.
/// </summary>
/// <param name="values">integer to write</param>
/// <param name="buffer">buffer to write to</param>
/// <param name="offset">position to start writing</param>
/// <param name="bytes">nr bytes to write</param>
/// <param name="littleEndian">little endian or big endian integer</param>
/// <returns></returns>
static bool BytesTool::writeBytes(long values, byte[] buffer, int offset, int bytes, bool littleEndian)
{
    if ((buffer != null)
            &&  (offset + bytes) <= buffer.Length
            &&  (bytes <= 8)
            &&  (bytes > 0))
    {
        for (int read=0;read < bytes;read++)
        {
            buffer[offset + (littleEndian ? read : (bytes-read-1))] = (byte) ((values >> (read << 3)) & 0xff);
        }
        return true;
    }
    return false;
}
/// <summary>
///	Function to write a string too a byte array at a given offset
/// </summary>
/// <param name="src">to read from</param>
/// <param name="buffer">to write the string too</param>
/// <param name="offset">position to start reading</param>
/// <param name="length">max length of string</param>
static void BytesTool::writeString(string src, byte[] buffer, int offset, int length)
{
    writeString(Encoding.ASCII, src, buffer, offset, length);
}
/// <summary>
///	Function to write a string too a byte array at a given offset
/// </summary>
/// <param name="enc">enconding type</param>
/// <param name="src">to read from</param>
/// <param name="buffer">to write the string too</param>
/// <param name="offset">position to start reading</param>
/// <param name="length">max length of string</param>
static void BytesTool::writeString(Encoding enc, string src, byte[] buffer, int offset, int length)
{
    if ((src != null)
            &&	(buffer != null))
    {
        int nrChars = enc.GetMaxCharCount((buffer.Length < (offset + length)) ? buffer.Length - offset : length);

        nrChars = (src.Length < nrChars) ? src.Length : nrChars;

        if (nrChars > 0)
            enc.GetBytes(src, 0, nrChars, buffer, offset);
    }
}
/// <summary>
/// Function to copy content of one buffer to another.
/// </summary>
/// <param name="dst">destination buffer</param>
/// <param name="offdst">offset in destination buffer</param>
/// <param name="src">source buffer</param>
/// <param name="offsrc">offset in source buffer</param>
/// <param name="length">number bytes to copy</param>
static int BytesTool::copy(byte[] dst, int offdst, byte[] src, int offsrc, int length)
{
    int loper=0;
    for (;(loper < length) && ((offdst + loper) < dst.Length) && ((offsrc + loper) < src.Length);loper++)
    {
        dst[offdst + loper] = src[offsrc + loper];
    }
    return loper;
}
/// <summary>
/// Function to empty a buffer to a defined number.
/// </summary>
/// <param name="buffer">destination buffer</param>
/// <param name="offset">offset in buffer</param>
/// <param name="nrbytes">number byte to empty</param>
/// <param name="type">number to empty to</param>
static void BytesTool::emptyBuffer(byte[] buffer, int offset, int nrbytes, byte type)
{
    for (int x=0;(x < nrbytes)&&((x + offset) < buffer.Length);x++)
    {
        buffer[offset + x] = type;
    }
}
}
