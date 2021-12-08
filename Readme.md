# 项目起源

受限于嵌入式平台，只能运行C和C++程序，在网上找不到C++相关的SCP-ECG格式实现，最后参考开源工程 [C# ECG Toolkit](https://sourceforge.net/projects/ecgtoolkit-cs/)，将C#代码转换为C++实现，完成了SCP-ECG格式的写入功能

# C# ECG Toolkit

ECG Toolkit support for: SCP-ECG, DICOM, HL7 aECG, ISHNE & MUSE-XML

https://sourceforge.net/projects/ecgtoolkit-cs/

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

## System.Text.Encoding处理