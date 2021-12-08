#ifndef _ZQ_IGLOBALMEASUREMENT_CS_
#define _ZQ_IGLOBALMEASUREMENT_CS_
    

namespace ECGConversion.ECGGlobalMeasurements
{
	/// <summary>
	/// Interface for manupalation of the measurements.
	/// </summary>
	class IGlobalMeasurement
	{
		/// <summary>
		/// Function to get the global measurements of ECG.
		/// </summary>
		/// <param name="mes">global measurements</param>
		/// <returns>0 on success</returns>
		virtual int getGlobalMeasurements(out GlobalMeasurements mes) = 0;
		/// <summary>
		/// Function to set the global measurements of ECG.
		/// </summary>
		/// <param name="mes">global measurements</param>
		/// <returns>0 on success</returns>
		virtual int setGlobalMeasurements(GlobalMeasurements mes) = 0;
	};
}


#endif  /*#ifndef _ZQ_IGLOBALMEASUREMENT_CS_*/
