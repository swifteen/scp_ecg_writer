#ifndef _ILEADMEASUREMENT_CS_
#define _ILEADMEASUREMENT_CS_
#include "LeadMeasurements.h"    

namespace ECGConversion
{
namespace ECGLeadMeasurements
{
	/// <summary>
	/// Interface for manupalation of the measurements.
	/// </summary>
	class ILeadMeasurement
	{
		/// <summary>
		/// Function to set the lead measurements of ECG.
		/// </summary>
		/// <param name="mes">lead measurements</param>
		/// <returns>0 on success</returns>
		virtual int setLeadMeasurements(LeadMeasurements& mes) = 0;
	};
}
}
#endif  /*#ifndef _ILEADMEASUREMENT_CS_*/
