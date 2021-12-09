#ifndef _ILEADMEASUREMENT_CS_
#define _ILEADMEASUREMENT_CS_
    

namespace ECGConversion.ECGLeadMeasurements
{
	/// <summary>
	/// Interface for manupalation of the measurements.
	/// </summary>
	class ILeadMeasurement
	{
		/// <summary>
		/// Function to get the lead measurements of ECG.
		/// </summary>
		/// <param name="mes">lead measurements</param>
		/// <returns>0 on success</returns>
		virtual int getLeadMeasurements(out LeadMeasurements mes) = 0;
		/// <summary>
		/// Function to set the lead measurements of ECG.
		/// </summary>
		/// <param name="mes">lead measurements</param>
		/// <returns>0 on success</returns>
		virtual int setLeadMeasurements(LeadMeasurements mes) = 0;
	};
}

#endif  /*#ifndef _ILEADMEASUREMENT_CS_*/
