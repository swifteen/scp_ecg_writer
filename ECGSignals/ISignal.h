#ifndef _ZQ_ISIGNAL_CS_
#define _ZQ_ISIGNAL_CS_
    

namespace ECGConversion.ECGSignals
{
	/// <summary>
	/// Interface for manupalation of the signals.
	/// </summary>
	class ISignal
	{
		/// <summary>
		/// Function to get the signal of an ECG.
		/// </summary>
		/// <param name="signals">signals</param>
		/// <returns>0 on succes</returns>
		virtual int getSignals(out Signals signals) = 0;
		/// <summary>
		/// Function to get the signals of an ECG and set a given Signals object.
		/// </summary>
		/// <param name="signals">signals</param>
		/// <returns>0 on success</returns>
		virtual int getSignalsToObj(Signals signals) = 0;
		/// <summary>
		/// Function to set the signals of an ECG.
		/// </summary>
		/// <param name="signals">signals</param>
		/// <returns>0 on success</returns>
		virtual int setSignals(Signals signals) = 0;
	};
}


#endif  /*#ifndef _ZQ_ISIGNAL_CS_*/
