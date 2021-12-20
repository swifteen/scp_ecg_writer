#ifndef _ECGTOOL_H_
#define _ECGTOOL_H_
#include "ScpGlobal.h"

namespace ECGConversion
{
/// <summary>
/// a tool for calculating some leads.
/// </summary>
class ECGTool
{
public:
#if 0
    /// <summary>
    /// Function to set an other multiplier (if this function is improperly used data will be lost).
    /// </summary>
    /// <param name="src">signal to change multiplier</param>
    /// <param name="srcmulti">orignal multiplier</param>
    /// <param name="dstmulti">preferred multiplier</param>
    /// <returns>0 on success</returns>
    static int ChangeMultiplier(short[][] src, double srcmulti, double dstmulti);
#endif
    /// <summary>
    /// Function to set an other multiplier (if this function is improperly used data will be lost).
    /// </summary>
    /// <param name="src">Lead to change multiplier</param>
    /// <param name="srcmulti">orignal multiplier</param>
    /// <param name="dstmulti">preferred multiplier</param>
    /// <returns>0 on success</returns>
    static int ChangeMultiplier(short* src, int srcLength, double srcmulti, double dstmulti);
};
}


#endif  /*#ifndef _ECGTOOL_H_*/
