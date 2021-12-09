#include "SCPFormat.h"
#include "SCPSection0.h"
#include "SCPSection1.h"
#include "SCPSection2.h"
#include "SCPSection3.h"
#include "SCPSection4.h"
#include "SCPSection5.h"
#include "SCPSection6.h"
#include "SCPSection7.h"
#include "SCPSection8.h"
#include "SCPSectionUnkown.h"
#include "SCPSection10.h"
#include "SCPSection11.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace ECGConversion.ECGSignals;
using namespace Communication_IO_Tools;

namespace ECGConversion.SCP
{
/// <summary>
/// Class containing the entire SCP format.
/// </summary>
// Static settings of format.
byte SCPFormat::DefaultSectionVersion = 20;
byte SCPFormat::DefaultProtocolVersion = 20;

SCPFormat::SCPFormat()
{
    _QRSSubtractionSupport = false;
    _BimodalCompressionUsed = false;
    _BimodalCompressionRate = 0;
    _EncodingType = EncodingType::DefaultHuffman;
    _DifferenceDataSection5Used = 2;
    _DifferenceDataSection6Used = 2;
    _UseLeadMeasurements = false;

    _MinFileLength = 158;
    _MinNrSections = 12;
    _MinNrWorkingSections = 2;
    // data structure of format.
    _CRC = 0;
    _Default = {	new SCPSection0(),
                    new SCPSection1(),
                    new SCPSection2(),
                    new SCPSection3(),
                    new SCPSection4(),
                    new SCPSection5(),
                    new SCPSection6(),
                    new SCPSection7(),
                    new SCPSection8(),
                    new SCPSectionUnknown(),
                    new SCPSection10(),
                    new SCPSection11()};
}

int SCPFormat::Write(string file)
{
	if (file != null)
	{
		// open stream to write to.
		Stream output = new FileStream(file, FileMode.Create);
		// use write function for streams.
		int ret = Write(output);
		// close stream after writing.
		output.Close();
		return ret << 1;
	}
	return 0x1;
}

int SCPFormat::Write(Stream output)
{
	if ((output != null)
	&&	(output.CanWrite))
	{
		// set pointers
		setPointers();
		byte[] buffer = new byte[getFileSize()];
		// use write function for byte arrays.
		int err = Write(buffer, 0);
		if (err == 0)
		{
			output.Write(buffer, 0, buffer.Length);
		}
		return err << 1;
	}
	return 0x1;
}

int SCPFormat::Write(byte[] buffer, int offset)
{
    // Check if format works.
    if (Works())
    {
        _Length = getFileSize();
        if ((buffer != null)
                &&  ((offset + _Length) <= buffer.Length)
                &&  (_Default[0] is SCPSection0))
        {
            // Write length of file.
            BytesTool::writeBytes(_Length, buffer, offset + sizeof(_CRC), sizeof(_Length), true);
            SCPSection0 pointers = (SCPSection0) _Default[0];
            // Write all sections in format.
            for (int loper=0;loper < pointers.getNrPointers();loper++)
            {
                if (loper < _MinNrSections)
                {
                    _Default[loper].Write(buffer, offset + pointers.getIndex(loper) - 1);
                }
//                else if ((pointers.getLength(loper) > SCPSection.Size)
//                         &&	 (_Manufactor[loper - _MinNrSections] != null))
//                {
//                    _Manufactor[loper - _MinNrSections].Write(buffer, offset + pointers.getIndex(loper) - 1);
//                }
            }
            // Calculate CRC of byte array.
            CRCTool crctool;
            crctool.Init(CRCTool::CRCCode::CRC_CCITT);
            _CRC = crctool.CalcCRCITT(buffer, offset + sizeof(_CRC), _Length - sizeof(_CRC));
            BytesTool::writeBytes(_CRC, buffer, offset, sizeof(_CRC), true);
            return 0x0;
        }
        return 0x2;
    }
    return 0x1;
}

int SCPFormat::getFileSize()
{
    if (Works()
            &&  (_Default[0] is SCPSection0))
    {
        SCPSection0 pointers = (SCPSection0) _Default[0];
        int sum = sizeof(_CRC) + sizeof(_Length);
        for (int loper=0;loper < pointers.getNrPointers();loper++)
        {
            sum += pointers.getLength(loper);
        }
        return sum;
    }
    return 0;
}

IDemographic SCPFormat::getDemographic()
{
    if (_Default[1] is SCPSection1)
        return (SCPSection1)_Default[1];

    return null;
}
IDiagnostic SCPFormat::getDiagnostic()
{
    if (_Default[8] is SCPSection8)
        return (SCPSection8) _Default[8];

    return null;
}
IGlobalMeasurement SCPFormat::getGlobalMeasurements()
{
    if (_Default[7] is SCPSection7)
        return (SCPSection7) _Default[7];

    return null;
}
ISignal SCPFormat::getSignals()
{
    return this;
}
ILeadMeasurement SCPFormat::getLeadMeasurements()
{
    if (_UseLeadMeasurements
            &&	(_Default[10] is SCPSection10))
        return (SCPSection10) _Default[10];

    return null;
}

bool SCPFormat::Works()
{
    if ((_Default.Length == _MinNrSections)
            &&  (_Default[0] is SCPSection0)
            &&  (_MinNrSections /* + (_Manufactor != null ? _Manufactor.Length : 0) */) == ((SCPSection0)_Default[0]).getNrPointers())
    {
        for (int loper=0;loper < _MinNrWorkingSections;loper++)
        {
            if (_Default[loper] == null)
            {
                return false;
            }
        }
        return (_Default[0].Works() &&  _Default[1].Works());
    }
    return false;
}

void SCPFormat::Empty()
{
    EmptyFormat();
}

int SCPFormat::setSignals(Signals signals)
{
    if ((signals != null)
            &&  (signals.NrLeads > 0))
    {
        // Decide wich encoding to use.
        switch (_EncodingType)
        {
        case EncodingType.None:
            ((SCPSection2)_Default[2]).UseNoHuffman();
            break;
        case EncodingType.OptimizedHuffman:
            // not implemented!
            ((SCPSection2)_Default[2]).UseStandard();
            break;
        case EncodingType.DefaultHuffman:
        default:
            ((SCPSection2)_Default[2]).UseStandard();
            break;
        }

        if (((ISignal)_Default[3]).setSignals(signals) != 0)
        {
            return 2;
        }

        SCPSection5 median = (SCPSection5) _Default[5];
        median.setAVM(signals.MedianAVM);
        median.setSamplesPerSecond(signals.MedianSamplesPerSecond);

        SCPSection6 rhythm = (SCPSection6) _Default[6];
        rhythm.setAVM(signals.RhythmAVM);
        rhythm.setSamplesPerSecond(signals.RhythmSamplesPerSecond);

        short[][] rhythmData = new short[signals.NrLeads][];
        short[][] medianData = new short[signals.NrLeads][];
        for (int loper=0;loper < signals.NrLeads;loper++)
        {
            if (signals[loper].Rhythm == null)
            {
                return 4;
            }
            rhythmData[loper] = signals[loper].Rhythm;
            if ((medianData == null)
                    ||  (signals[loper].Median == null))
            {
                medianData = null;
            }
            else
            {
                medianData[loper] = signals[loper].Median;
            }
        }

        if (medianData != null)
        {
            if (((ISignal)_Default[4]).setSignals(signals) != 0)
            {
                return 8;
            }

            if (signals.MedianSamplesPerSecond < signals.RhythmSamplesPerSecond )
            {
                median.setSamplesPerSecond(signals.RhythmSamplesPerSecond);
                ECGTool.ResampleSignal(medianData, signals.MedianSamplesPerSecond, signals.RhythmSamplesPerSecond, out medianData);
            }

            if (median.EncodeData(medianData, (SCPSection2) _Default[2], (ushort)((signals.MedianLength * signals.MedianSamplesPerSecond) / 1000), (_EncodingType == EncodingType.None ? (byte)0 : _DifferenceDataSection5Used)) != 0)
            {
                return 16;
            }

            if (signals.QRSZone != null)
            {
                if (signals.RhythmAVM <= signals.MedianAVM)
                {
                    ECGTool.ChangeMultiplier(medianData, signals.MedianAVM, signals.RhythmAVM);
                }
                else
                {
                    ECGTool.ChangeMultiplier(rhythmData, signals.RhythmAVM, signals.MedianAVM);
                    rhythm.setAVM(signals.MedianAVM);
                }
            }

            ECGTool.ResampleSignal(rhythmData, signals.RhythmSamplesPerSecond, median.getSamplesPerSecond(), out rhythmData);

            if (_QRSSubtractionSupport
                    &&	(signals.QRSZone != null))
            {
                ((SCPSection3) _Default[3]).setMediansUsed(true);
                ((SCPSection4) _Default[4]).SubtractMedians((SCPSection3) _Default[3], rhythmData, medianData);
            }
        }

        if (_BimodalCompressionUsed
                &&	(_BimodalCompressionRate > 0)
                &&	(medianData != null)
                &&	(signals.QRSZone != null))
        {
            // Bimodal Compression must be set in set section 6.
            rhythm.setBimodal(true);
            rhythm.setSamplesPerSecond(signals.MedianSamplesPerSecond / _BimodalCompressionRate);

            // Determine QRS zones for bimodal compression
            GlobalMeasurements global;
            ((IGlobalMeasurement)_Default[7]).getGlobalMeasurements(out global);
            ((SCPSection4)_Default[4]).setProtected(global, median.getSamplesPerSecond(), _BimodalCompressionRate, ((SCPSection3)_Default[3]).getMinBegin(), ((SCPSection3)_Default[3]).getMaxEnd());
        }

        if (rhythm.EncodeData(rhythmData, (SCPSection2) _Default[2], (SCPSection3) _Default[3], (SCPSection4) _Default[4], signals.MedianSamplesPerSecond, (_EncodingType == EncodingType.None ? (byte)0 : _DifferenceDataSection6Used)) != 0)
        {
            return 32;
        }
        return 0;
    }
    return 1;
}

/// <summary>
/// Function to set pointers.
/// </summary>
void SCPFormat::setPointers()
{
    if (_Default[0] is SCPSection0)
    {
        SCPSection0 pointers = (SCPSection0) _Default[0];
        pointers.setNrPointers(_MinNrSections /* + (_Manufactor != null ? _Manufactor.Length : 0) */);
        int sum = sizeof(_CRC) + sizeof(_Length) + 1;
        for (int loper=0;loper < _MinNrSections;loper++)
        {
            ushort id = (ushort) loper;
            int length = _Default[loper].getLength();
            int index = (length > SCPSection.Size ? sum : 0);
            pointers.setPointer(loper, id, length, index);
            sum += length;
        }
#if 0
        if (_Manufactor != null)
        {
            for (int loper=0;loper < _Manufactor.Length;loper++)
            {
                ushort id = _Manufactor[loper].getSectionID();
                int length = _Manufactor[loper].getLength();
                int index = (length > SCPSection.Size ? sum : 0);
                pointers.setPointer(_MinNrSections + loper, id, length, index);
                sum += length;
            }
        }
#endif

        _Length = sum - 1;

        // Determine file Protocol Compatibility Level.
        if (_Default[0].Works()
                &&  _Default[1].Works()
                &&  (_Default[1] is SCPSection1))
        {
            if (_Default[2].Works()
                    &&  _Default[3].Works())
            {
                if (_Default[4].Works()
                        &&  _Default[5].Works()
                        &&  _Default[6].Works())
                {
                    ((SCPSection1)_Default[1]).setProtocolCompatibilityLevel(SCPSection1.ProtocolCompatibility.CatIV);
                }
                else if (_Default[5].Works())
                {
                    ((SCPSection1)_Default[1]).setProtocolCompatibilityLevel(SCPSection1.ProtocolCompatibility.CatIII);
                }
                else if (_Default[6].Works())
                {
                    ((SCPSection1)_Default[1]).setProtocolCompatibilityLevel(SCPSection1.ProtocolCompatibility.CatII);
                }
                else if (_Default[7].Works()
                         &&   _Default[8].Works())
                {
                    ((SCPSection1)_Default[1]).setProtocolCompatibilityLevel(SCPSection1.ProtocolCompatibility.CatI);
                }
            }
            else if (_Default[7].Works()
                     &&   _Default[8].Works())
            {
                ((SCPSection1)_Default[1]).setProtocolCompatibilityLevel(SCPSection1.ProtocolCompatibility.CatI);
            }
        }
    }
}
/// <summary>
/// Function to empty entire format.
/// </summary>
private void SCPFormat::EmptyFormat()
{
    for (int loper=0;loper < _MinNrSections;loper++)
    {
        _Default[loper].Empty();
    }
//    _Manufactor = null;
}

//region IDisposable Members
void SCPFormat::Dispose()
{
    //			base.Dispose();

    _CRC = 0;
    _Length = 0;
    if (_Default != null)
    {
        for (int loper=0;loper < _Default.Length;loper++)
        {
            if (_Default[loper] != null)
            {
                _Default[loper].Empty();
                _Default[loper] = null;
            }
        }
        _Default = null;
    }
}

/// <summary>
/// Function to convert to SCP.
/// </summary>
/// <param name="src">an ECG file to convert</param>
/// <param name="dst">SCP file returned</param>
/// <returns>0 on success</returns>
static int SCPFormat::ToSCP(IECGFormat src, ECGConfig cfg, out IECGFormat dst)
{
    dst = null;
    if (src != null)
    {
        dst = new SCPFormat();

        if ((cfg != null)
                &&	((dst.Config == null)
                     ||	 !dst.Config.Set(cfg)))
            return 1;

        int err = ECGConverter.Convert(src, dst);
        if (err != 0)
        {
            return err;
        }

        ((SCPFormat)dst).setPointers();

        return 0;
    }
    return 1;
}
}
}
