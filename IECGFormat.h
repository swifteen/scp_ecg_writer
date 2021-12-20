#ifndef _IECGFORMAT_H_
#define _IECGFORMAT_H_
#include "ScpGlobal.h"
#include "ISignal.h"
#include "IDemographic.h"
#include "IDiagnostic.h"
#include "IGlobalMeasurement.h"
#include "ILeadMeasurement.h"

using namespace ECGConversion::ECGDemographics;
using namespace ECGConversion::ECGDiagnostic;
using namespace ECGConversion::ECGGlobalMeasurements;
using namespace ECGConversion::ECGLeadMeasurements;
using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{
class IECGFormat
{
public:
    /// <summary>
    /// Function to write ECG format to byte array.
    /// </summary>
    /// <param name="buffer">byte array to write to</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    virtual int Write(uchar* buffer, int bufferLength, int offset) = 0;
    /// <summary>
    /// Function to get IDemographic class.
    /// </summary>
    /// <returns>IDemographic object</returns>
    virtual IDemographic* getDemographic() = 0;
    /// <summary>
    /// Function to get IDiagnostic class.
    /// </summary>
    /// <returns>IDiagnostic object</returns>
    virtual IDiagnostic* getDiagnostic() = 0;
    /// <summary>
    /// Function to get IGlobalMeasurement class.
    /// </summary>
    /// <returns>IGlobalMeasurement object</returns>
    virtual IGlobalMeasurement* getGlobalMeasurements() = 0;
    /// <summary>
    /// Function to get ISignal class.
    /// </summary>
    /// <returns></returns>
    virtual ISignal* getSignals() = 0;
    /// <summary>
    /// Function to get IMeasurement class.
    /// </summary>
    /// <returns>ILeadMeasurement object</returns>
    virtual ILeadMeasurement* getLeadMeasurements() = 0;
    /// <summary>
    /// Function to determine size of file.
    /// </summary>
    /// <returns>size of file</returns>
    virtual int getFileSize() = 0;
    /// <summary>
    /// Function to check if format works.
    /// </summary>
    /// <returns>true if format works</returns>
    virtual bool Works() = 0;
    /// <summary>
    /// Function to empty a ECG file.
    /// </summary>
    virtual void Empty() = 0;
    //region IDisposable Members
    virtual void Dispose() = 0;
};
}


#endif  /*#ifndef _IECGFORMAT_H_*/
