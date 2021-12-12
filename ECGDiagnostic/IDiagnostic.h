#ifndef _IDIAGNOSTIC_CS_
#define _IDIAGNOSTIC_CS_
#include "Statements.h"
namespace ECGConversion
{

namespace ECGDiagnostic
{

	/// <summary>
	/// Interface for manupalation of the diagnostic statements.
	/// </summary>
	class IDiagnostic
	{
		public:
#if 0
		/// <summary>
		/// Function to get the diagnostic statements.
		/// </summary>
		/// <param name="stat">the diagnostic statements</param>
		/// <returns>0 on success</returns></returns>
		virtual int getDiagnosticStatements(Statements stat) = 0;
#endif
		/// <summary>
		/// Function to set the diagnostic statements.
		/// </summary>
		/// <param name="stat">the diagnostic statements to set</param>
		/// <returns>0 on success</returns></returns>
		virtual int setDiagnosticStatements(Statements& stat) = 0;
	};
}
}


#endif  /*#ifndef _IDIAGNOSTIC_CS_*/
