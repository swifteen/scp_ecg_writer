#ifndef _ZQ_IDIAGNOSTIC_CS_
#define _ZQ_IDIAGNOSTIC_CS_

namespace ECGConversion.ECGDiagnostic
{
	/// <summary>
	/// Interface for manupalation of the diagnostic statements.
	/// </summary>
	class IDiagnostic
	{
		/// <summary>
		/// Function to get the diagnostic statements.
		/// </summary>
		/// <param name="stat">the diagnostic statements</param>
		/// <returns>0 on success</returns></returns>
		virtual int getDiagnosticStatements(out Statements stat) = 0;
		/// <summary>
		/// Function to set the diagnostic statements.
		/// </summary>
		/// <param name="stat">the diagnostic statements to set</param>
		/// <returns>0 on success</returns></returns>
		virtual int setDiagnosticStatements(Statements stat) = 0;
	};
}


#endif  /*#ifndef _ZQ_IDIAGNOSTIC_CS_*/
