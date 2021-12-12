[TOC]

# 项目起源

受限于嵌入式平台，只能运行C和C++程序，在网上找不到C++相关的SCP-ECG格式实现，最后参考开源工程 [C# ECG Toolkit](https://sourceforge.net/projects/ecgtoolkit-cs/)，将C#代码转换为C++实现，完成了SCP-ECG格式的写入功能

# C# ECG Toolkit

ECG Toolkit support for: SCP-ECG, DICOM, HL7 aECG, ISHNE & MUSE-XML

https://sourceforge.net/projects/ecgtoolkit-cs/

# 项目依赖

为了支持多字符集，需要使用[iconv](https://www.gnu.org/software/libiconv/)

```c++

```



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

将字符集相关的代码放到最后再做，如下，先用#if0包起来，注释为TODO

```c++
#if 0 //TODO
    /// <summary>
    ///	Function to write a string too a byte array at a given offset
    /// </summary>
    /// <param name="src">to read from</param>
    /// <param name="buffer">to write the string too</param>
    /// <param name="offset">position to start reading</param>
    /// <param name="length">max length of string</param>
    static void writeString(string src, byte[] buffer, int offset, int length);

    /// <summary>
    ///	Function to write a string too a byte array at a given offset
    /// </summary>
    /// <param name="enc">enconding type</param>
    /// <param name="src">to read from</param>
    /// <param name="buffer">to write the string too</param>
    /// <param name="offset">position to start reading</param>
    /// <param name="length">max length of string</param>
    static void writeString(Encoding enc, string src, byte[] buffer, int offset, int length);
#endif
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