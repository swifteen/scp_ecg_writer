#ifndef _ISIGNAL_CS_
#define _ISIGNAL_CS_
#include "Signals.h"

namespace ECGConversion
{
namespace ECGSignals
{
	/// <summary>
	/// Interface for manupalation of the signals.
	/// </summary>
	class ISignal
	{
		public:
		/// <summary>
		/// Function to set the signals of an ECG.
		/// </summary>
		/// <param name="signals">signals</param>
		/// <returns>0 on success</returns>
		virtual int setSignals(Signals& signals) = 0;
	};
}
}

#endif  /*#ifndef _ISIGNAL_CS_*/
