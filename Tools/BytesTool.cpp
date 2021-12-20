#include "BytesTool.h"
#include <cstring>
#include <iconv.h>

namespace Communication_IO_Tools
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
bool BytesTool::writeBytes(long values, uchar* buffer, int bufferLength, int offset, int bytes, bool littleEndian)
{
    if ((buffer != null)
        && (offset + bytes) <= bufferLength
        && (bytes <= 8)
        && (bytes > 0)) {
        for (int read = 0; read < bytes; read++) {
            buffer[offset + (littleEndian ? read : (bytes - read - 1))] = (uchar)((values >> (read << 3)) & 0xff);
        }

        SCP_PD("writeBytes ok,values:%ld,bufferLength:%d,offset:%d,bytes:%d\n", values, bufferLength, offset, bytes);
        return true;
    }

    SCP_PE("writeBytes error,values:%ld,bufferLength:%d,offset:%d,bytes:%d\n", values, bufferLength, offset, bytes);
    return false;
}

/// <summary>
/// Function to write a string too a byte array at a given offset
/// </summary>
/// <param name="src">to read from</param>
/// <param name="buffer">to write the string too</param>
/// <param name="offset">position to start reading</param>
/// <param name="length">max length of string</param>
void BytesTool::writeString(const std::string& src, uchar* buffer, int bufferLength, int offset, int length)
{
    writeString("ASCII", src, buffer, bufferLength, offset, length);
}
/// <summary>
/// Function to write a string too a byte array at a given offset
/// </summary>
/// <param name="enc">enconding type</param>
/// <param name="src">to read from</param>
/// <param name="buffer">to write the string too</param>
/// <param name="offset">position to start reading</param>
/// <param name="length">max length of string</param>
void BytesTool::writeString(const std::string& dstEncoding,
                            const std::string& src,
                            uchar* buffer,
                            int bufferLength,
                            int offset,
                            int length)
{
    if ((src.length() > 0) && (buffer != null)) {
        char* src_cstr = new char [src.length() + 1];

        if (src_cstr == null) {
            return;
        }

        snprintf(src_cstr, src.length() + 1, "%s", src.c_str());
        size_t src_cstr_len = strlen(src_cstr);
        //The maximum number of characters produced by decoding the specified number of bytes.
        size_t nrChars = (bufferLength < (offset + length)) ? bufferLength - offset : length;
        nrChars = (src.length() < nrChars) ? src.length() : nrChars;

        if (nrChars > 0) {
            char* dst_cstr = new char[nrChars];

            if (dst_cstr == null) {
                delete[] src_cstr;
                return;
            }

            bool convRet = false;

            do {
                iconv_t icd = iconv_open(dstEncoding.c_str(), "UTF−8");

                if ((iconv_t) - 1 == icd) {
                    SCP_PE("iconv_open error,dstEncoding[%s],errno[%d],src_cstr[%s]\n", dstEncoding.c_str(), errno, src_cstr);
                    break;
                }

                memset(dst_cstr, 0, nrChars);
                int ret = iconv(icd, &src_cstr, &src_cstr_len, &dst_cstr, &nrChars);

                if (0 != ret) {
                    SCP_PE("iconv error,ret[%d]dstEncoding[%s]src_cstr[%s]\n", ret, dstEncoding.c_str(), src_cstr);
                    break;
                }

                iconv_close(icd);
                convRet = true;
            }
            while (0);

            if (convRet) {
                memcpy(buffer + offset, dst_cstr, nrChars);
            }
            else {
                memcpy(buffer + offset, src_cstr, nrChars);
            }

            delete[] dst_cstr;
        }

        delete[] src_cstr;
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
int BytesTool::copy(uchar* dst, int dstLength, int offdst, const uchar* src, int srcLength, int offsrc, int length)
{
    int loper = 0;

    for (; (loper < length) && ((offdst + loper) < dstLength) && ((offsrc + loper) < srcLength); loper++) {
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
void BytesTool::emptyBuffer(uchar* buffer, int bufferLength, int offset, int nrbytes, uchar type)
{
    for (int x = 0; (x < nrbytes) && ((x + offset) < bufferLength); x++) {
        buffer[offset + x] = type;
    }
}
}
