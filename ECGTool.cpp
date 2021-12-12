#include "ECGTool.h"    

namespace ECGConversion
{
#if 0
/// <summary>
/// Function to set an other multiplier (if this function is improperly used data will be lost).
/// </summary>
/// <param name="src">signal to change multiplier</param>
/// <param name="srcmulti">orignal multiplier</param>
/// <param name="dstmulti">preferred multiplier</param>
/// <returns>0 on success</returns>
int ECGTool::ChangeMultiplier(short[][] src, double srcmulti, double dstmulti)
{
	if ((src != null)
	&&	(srcmulti == dstmulti))
	{
		return 0;
	}
	else if ((src != null)
		&&   (srcmulti > 0)
		&&   (dstmulti > 0))
	{
		for (int loper=0;loper < src.Length;loper++)
		{
			if (ChangeMultiplier(src[loper], srcmulti, dstmulti) != 0)
			{
				return (0x2 << loper);
			}
		}
		return 0;
	}
	return 1;
}
#endif
/// <summary>
/// Function to set an other multiplier (if this function is improperly used data will be lost).
/// </summary>
/// <param name="src">Lead to change multiplier</param>
/// <param name="srcmulti">orignal multiplier</param>
/// <param name="dstmulti">preferred multiplier</param>
/// <returns>0 on success</returns>
int ECGTool::ChangeMultiplier(short* src, int srcLength,double srcmulti, double dstmulti)
{
	if ((src != null)
	&&	(srcmulti == dstmulti))
	{
		return 0;
	}
	else if ((src != null)
		&&   (srcmulti > 0)
		&&   (dstmulti > 0))
	{

		short* ps = src;
		short* psend = ps + srcLength;

		while (ps < psend)
		{
			*ps = (short) ((*ps * srcmulti) / dstmulti);
			ps++;
		}
		return 0;
	}
	return 1;
}
}
