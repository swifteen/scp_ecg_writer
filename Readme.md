[TOC]

# 项目起源

受限于嵌入式平台，只能运行C和C++程序，在网上找不到C++相关的SCP-ECG格式实现，最后参考开源工程 [C# ECG Toolkit](https://sourceforge.net/projects/ecgtoolkit-cs/)，将C#代码转换为C++实现，仅仅只完成了SCP-ECG格式的写入功能

# C# ECG Toolkit

ECG Toolkit support for: SCP-ECG, DICOM, HL7 aECG, ISHNE & MUSE-XML

https://sourceforge.net/projects/ecgtoolkit-cs/

![RecordOverview](.\SCP-ECG Figure\SCP-ECG class.svg)

# 项目依赖

为了支持多字符集，需要使用[iconv](https://www.gnu.org/software/libiconv/)

# 例子代码

见根目录文件TestScpEcgWriter.cpp

例子生成文件见SCP-ECG example目录下test.scp

# C#转换为C++实现细节

## Byte类型处理

C#中Byte类型定义

> [Byte](https://docs.microsoft.com/en-us/dotnet/api/system.byte?view=net-6.0) is an immutable value type that represents unsigned integers with values that range from 0 (which is represented by the [Byte.MinValue](https://docs.microsoft.com/en-us/dotnet/api/system.byte.minvalue?view=net-6.0) constant) to 255 (which is represented by the [Byte.MaxValue](https://docs.microsoft.com/en-us/dotnet/api/system.byte.maxvalue?view=net-6.0) constant). 

```c#
/// <summary>
/// Function to write an integer to a buffer at an offset.
/// </summary>
/// <param name="values">integer to write</param>
/// <param name="buffer">buffer to write to</param>
/// <param name="offset">position to start writing</param>
/// <param name="bytes">nr bytes to write</param>
/// <param name="littleEndian">little endian or big endian integer</param>
/// <returns></returns>
public static bool writeBytes(long values, byte[] buffer, int offset, int bytes, bool littleEndian)
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
```

转换为C++后

1. 将参数 byte[] buffer替换为两个参数 uchar* buffer,int bufferLength,
2. 将buffer.Length 替换为新添加的输入参数bufferLength
3. 将byte修改为uchar类型，在ScpGlobal.h文件中定义

```c++
bool BytesTool::writeBytes(long values, uchar* buffer,int bufferLength, int offset, int bytes, bool littleEndian)
{
    if ((buffer != null)
            &&  (offset + bytes) <= bufferLength
            &&  (bytes <= 8)
            &&  (bytes > 0))
    {
        for (int read=0;read < bytes;read++)
        {
            buffer[offset + (littleEndian ? read : (bytes-read-1))] = (uchar) ((values >> (read << 3)) & 0xff);
        }
        return true;
    }
    return false;
}
```

## 类型定义和公共的头文件处理

ScpGlobal.h文件如下，用来定义类型和包含公共的头文件

```c++
#ifndef _SCPGLOBAL_H_
#define _SCPGLOBAL_H_
//公共的头文件
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <memory.h>

//定义类型
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;

#define uchar_MIN (0)
#define uchar_MAX (uchar(-1))

#define ushort_MinValue (0)
#define ushort_MaxValue (ushort(-1))

#define uint_MIN (0)
#define uint_MAX (uint(-1))

#define ulong_MIN (0)
#define ulong_MAX (ulong(-1L))

#define ulong_MIN (0)
#define ulong_MAX (ulong(-1L))

#define Int32_MaxValue ((int)0x7FFFFFFF)
#define Int32_MinValue ((int)0x80000000)

#define null             0L

#endif  /*#ifndef _SCPGLOBAL_H_*/
```

## 批量替换目录下所有字符串

```shell
sed -i "s/Communication.IO.Tools/Communication::IO::Tools/g" `grep "Communication.IO.Tools" -rl ./`
```

命令分解：

1、获取当前目录下所有包含“Communication.IO.Tools"字符串的文件

```shell
grep "Communication.IO.Tools" -rl ./
```

2、将文件$file中的“Communication.IO.Tools"字符串替换为”Communication::IO::Tools“

```shell
sed -i "s/Communication.IO.Tools/Communication::IO::Tools/g"  $file
```

## System.Text.Encoding处理

使用iconv开源库进行字符集转换

```c++
void BytesTool::writeString(const std::string& dstEncoding,
                            const std::string& src,
                            uchar* buffer,
                            int bufferLength,
                            int offset,
                            int length)
{
    if ((src.length() > 0) && (buffer != null)) {
        char* src_cstr = new char [src.length() + 1];
        char* tmp_src_cstr = src_cstr;

        if (src_cstr == null) {
            return;
        }

        memset(src_cstr, 0, src.length() + 1);
        snprintf(src_cstr, src.length() + 1, "%s", src.c_str());
        size_t src_cstr_len = strlen(src_cstr) + 1;
        //The maximum number of characters produced by decoding the specified number of bytes.
        size_t nrChars = (bufferLength < (offset + length)) ? bufferLength - offset : length;
        nrChars = (src.length() < nrChars) ? src.length() : nrChars;

        if (nrChars > 0) {
            /* Assign enough space to put the UTF-8. */
            size_t outbytesleft = 2 * nrChars;
            char* outbuf = new char[outbytesleft];

            if (outbuf == null) {
                delete[] src_cstr;
                return;
            }

            memset(outbuf, 0, outbytesleft);
            int ret = convert_charset("UTF-8", dstEncoding.c_str(), src_cstr, src_cstr_len, outbuf, &outbytesleft);

            if (0 == ret) {
                memcpy(buffer + offset, outbuf, outbytesleft);
            }
            else
            {
                SCP_PD("iconv failed: in string '%s', length %d, "
                       "out string '%s', length %d",
                       src_cstr, (int)src_cstr_len, outbuf, (int)outbytesleft);
                memcpy(buffer + offset, src_cstr, src_cstr_len);
            }

            delete[] outbuf;
        }

        delete[] src_cstr;
    }
}

int BytesTool::convert_charset(const char* from_charset,
                               const char* to_charset,
                               char* src_buf,
                               size_t src_len,
                               char* dst_buf,
                               size_t* p_dst_len)
{
    iconv_t icd = iconv_open(to_charset, from_charset);

    if ((iconv_t) - 1 == icd) {
        /* Initialization failure. */
        if (errno == EINVAL) {
            SCP_PE("Conversion from '%s' to '%s' is not supported.\n", from_charset, to_charset);
        }
        else {
            SCP_PE("Initialization failure: %s\n", strerror(errno));
        }

        return -1;
    }

    char** pin = &src_buf;
    char** pout = &dst_buf;
    size_t iconv_value = iconv(icd, pin, &src_len, pout, p_dst_len);

    if (iconv_value == (size_t) - 1) {
        switch (errno) {
            /* See "man 3 iconv" for an explanation. */
            case EILSEQ:
                SCP_PE("Invalid multibyte sequence.\n");
                break;

            case EINVAL:
                SCP_PE("Incomplete multibyte sequence.\n");
                break;

            case E2BIG:
                SCP_PE("No more room.\n");
                break;

            default:
                SCP_PE("Error: %s.\n", strerror(errno));
                break;
        }

        iconv_close(icd);
        return -1;
    }

    iconv_close(icd);
    return 0;
}
```

## 二维数组处理

There are three ways to pass a 2D array to a function:

1. The parameter is a 2D array

   ```cpp
   int array[10][10];
   void passFunc(int a[][10])
   {
       // ...
   }
   passFunc(array);
   ```

2. The parameter is an array containing pointers

   ```cpp
   int *array[10];
   for(int i = 0; i < 10; i++)
       array[i] = new int[10];
   void passFunc(int *a[10]) //Array containing pointers
   {
       // ...
   }
   passFunc(array);
   ```

3. The parameter is a pointer to a pointer

   ```cpp
   int **array;
   array = new int *[10];
   for(int i = 0; i <10; i++)
       array[i] = new int[10];
   void passFunc(int **a)
   {
       // ...
   }
   passFunc(array);
   ```

## 带指针的类处理

1. std:vector执行resize()操作时，如果超出了原有容量了，需要搬移原有成员时，会调用拷贝构造将原有成员拷贝到新的内存空间，而新增加的成员是会调用默认构造 
2. std:vector执行reserve()不会调用任何构造
3. 拷贝构造默认不会首先调用默认构造，不会首先将成员变量执行初始化，此时成员变量仍然为随机值。默认的拷贝构造会直接将输入类的变量直接等于赋值 
4. 只要包含指针的类，就要考虑拷贝构造和赋值构造，防止double free，要么是将拷贝构造和赋值构造设置为私有的，禁止拷贝构造和赋值构造，要么重写拷贝构造和赋值构造，进行深拷贝动作
5. 只要写好了构造，析构，拷贝构造，赋值构造，不用担当std容器操作导致的内存泄漏问题

The [`resize()`](http://en.cppreference.com/w/cpp/container/vector/resize) method (and passing argument to constructor is equivalent to that) will insert or delete appropriate number of elements to the vector to make it given size (it has optional second argument to specify their value). It will affect the `size()`, iteration will go over all those elements, push_back will insert after them and you can directly access them using the `operator[]`.

The [`reserve()`](http://en.cppreference.com/w/cpp/container/vector/reserve) method only allocates memory, but leaves it uninitialized. It only affects `capacity()`, but `size()` will be unchanged. There is no value for the objects, because nothing is added to the vector. If you then insert the elements, no reallocation will happen, because it was done in advance, but that's the only effect.

So it depends on what you want. If you want an array of 1000 default items, use `resize()`. If you want an array to which you expect to insert 1000 items and want to avoid a couple of allocations, use `reserve()`.

# TODO

1. 支持单个导联测量参数
2. 支持字符集配置