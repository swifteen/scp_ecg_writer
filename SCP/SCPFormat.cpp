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

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGSignals;
using namespace ECGConversion::ECGDemographics;
using namespace ECGConversion::ECGDiagnostic;	
using namespace ECGConversion::ECGGlobalMeasurements;
using namespace ECGConversion::ECGLeadMeasurements;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class containing the entire SCP format.
/// </summary>
// Static settings of format.
uchar SCPFormat::DefaultSectionVersion = 20;
uchar SCPFormat::DefaultProtocolVersion = 20;

SCPFormat::SCPFormat()
{
    _QRSSubtractionSupport = false;
    _BimodalCompressionUsed = false;
    _BimodalCompressionRate = 0;
    _EncodingType = EncodingTypeDefaultHuffman;
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
SCPFormat::~SCPFormat()
{
	int size = _Default.size();
	for (int i = 0; i < size; i++)
	{
		delete _Default[i];
	}
}

#if 0//todo
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
#endif

int SCPFormat::Write(uchar* buffer, int bufferLength, int offset)
{
    // Check if format works.
    if (Works())
    {
        _Length = getFileSize();		
		SCPSection0* pointers = dynamic_cast<SCPSection0*>(_Default[0]);
        if ((buffer != null)
                &&  ((offset + _Length) <= bufferLength)
                &&  (pointers != null))
        {
            // Write length of file.
            BytesTool::writeBytes(_Length, buffer, bufferLength,offset + sizeof(_CRC), sizeof(_Length), true);
            // Write all sections in format.
            for (int loper=0;loper < pointers->getNrPointers();loper++)
            {
                if (loper < _MinNrSections)
                {
                    _Default[loper]->Write(buffer, bufferLength,offset + pointers->getIndex(loper) - 1);
                }
#if 0 //current not support _Manufactor
                else if ((pointers.getLength(loper) > SCPSection.Size)
                         &&	 (_Manufactor[loper - _MinNrSections] != null))
                {
                    _Manufactor[loper - _MinNrSections].Write(buffer, offset + pointers.getIndex(loper) - 1);
                }
#endif
            }
            // Calculate CRC of byte array.
            CRCTool crctool;
            crctool.Init(CRCTool::CRCCode::CRC_CCITT);
            _CRC = crctool.CalcCRCITT(buffer, bufferLength,offset + sizeof(_CRC), _Length - sizeof(_CRC));
            BytesTool::writeBytes(_CRC, buffer, bufferLength,offset, sizeof(_CRC), true);
            return 0x0;
        }
        return 0x2;
    }
    return 0x1;
}

int SCPFormat::getFileSize()
{
	SCPSection0* pointers = dynamic_cast<SCPSection0*>(_Default[0]);

    if (Works()
            &&  (pointers != null))
    {
        int sum = sizeof(_CRC) + sizeof(_Length);
        for (int loper=0;loper < pointers->getNrPointers();loper++)
        {
            sum += pointers->getLength(loper);
        }
        return sum;
    }
    return 0;
}

IDemographic* SCPFormat::getDemographic()
{
	SCPSection1* section1 = dynamic_cast<SCPSection1*>(_Default[1]);
	if(section1 != null)
	{
		return section1;
	}

    return null;
}
IDiagnostic* SCPFormat::getDiagnostic()
{
	SCPSection8* section8 = dynamic_cast<SCPSection8*>(_Default[8]);
	if(section8 != null)
	{
		return section8;
	}
    return null;
}
IGlobalMeasurement* SCPFormat::getGlobalMeasurements()
{
	SCPSection7* section7 = dynamic_cast<SCPSection7*>(_Default[7]);
	if(section7 != null)
	{
		return section7;
	}
    return null;
}
ISignal* SCPFormat::getSignals()
{
    return this;
}
ILeadMeasurement* SCPFormat::getLeadMeasurements()
{
	SCPSection10* section10 = dynamic_cast<SCPSection10*>(_Default[10]);
	if(section10 != null)
	{
		return section10;
	}
    return null;
}

bool SCPFormat::Works()
{
	SCPSection0* pointers = dynamic_cast<SCPSection0*>(_Default[0]);
    if ((_Default.size() == _MinNrSections)
            &&  (pointers != null)
            &&  (_MinNrSections /* + (_Manufactor != null ? _Manufactor.Length : 0) */) == pointers->getNrPointers())
    {
        for (int loper=0;loper < _MinNrWorkingSections;loper++)
        {
            if (_Default[loper] == null)
            {
                return false;
            }
        }
        return (_Default[0]->Works() &&  _Default[1]->Works());
    }
    return false;
}

void SCPFormat::Empty()
{
    EmptyFormat();
}

int SCPFormat::setSignals(Signals& signals)
{
	int NrLeads = signals.getNrLeads();
    if (NrLeads > 0)
    {
		SCPSection2* section2= dynamic_cast<SCPSection2*>(_Default[2]);
		if(section2 != null)
		{
			// Decide wich encoding to use.
			switch (_EncodingType)
			{
			case EncodingTypeNone:
				section2->UseNoHuffman();
				break;
			case EncodingTypeOptimizedHuffman:
				// not implemented!
				section2->UseStandard();
				break;
			case EncodingTypeDefaultHuffman:
			default:
				section2->UseStandard();
				break;
			}
		}
		SCPSection3* section3= dynamic_cast<SCPSection3*>(_Default[3]);
		if((section3 == null) || (section3->setSignals(signals) != 0))
		{
            return 2;
		}

		SCPSection5* section5= dynamic_cast<SCPSection5*>(_Default[5]);
		if(section5 != null)
		{
			section5->setAVM(signals.MedianAVM);
			section5->setSamplesPerSecond(signals.MedianSamplesPerSecond);
		}
		SCPSection6* section6= dynamic_cast<SCPSection6*>(_Default[6]);
		if(section6 != null)
		{
			section6->setAVM(signals.RhythmAVM);
			section6->setSamplesPerSecond(signals.RhythmSamplesPerSecond);
		}

		std::vector<short*> rhythmData;		
		std::vector<int> rhythmDataLength;
		rhythmData.resize(NrLeads);
		rhythmDataLength.resize(NrLeads);
//        short[][] medianData = new short[signals.NrLeads][];
        for (int loper=0;loper < NrLeads;loper++)
        {
            if (signals[loper].Rhythm == null)
            {
                return 4;
            }
            rhythmData[loper] = signals[loper].Rhythm;
            rhythmDataLength[loper] = signals[loper].RhythmLength;
#if 0//current only support rhythm data
            if ((medianData == null)
                    ||  (signals[loper].Median == null))
            {
                medianData = null;
            }
            else
            {
                medianData[loper] = signals[loper].Median;
            }
#endif
        }

#if 0 //current only support rhythm data
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
#endif

		SCPSection4* section4= dynamic_cast<SCPSection4*>(_Default[4]);

        if ((section4 == null) 
			||(section6 == null) 
			|| (section6->EncodeData(rhythmData, 
									rhythmDataLength,
									section2,
									section3,
									section4, 
									signals.MedianSamplesPerSecond, 
									(_EncodingType == EncodingTypeNone ? (uchar)0 : _DifferenceDataSection6Used)) != 0))
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
	SCPSection0* pointers = dynamic_cast<SCPSection0*>(_Default[0]);

    if (pointers != null)
    {
        pointers->setNrPointers(_MinNrSections /* + (_Manufactor != null ? _Manufactor.Length : 0) */);
        int sum = sizeof(_CRC) + sizeof(_Length) + 1;
        for (int loper=0;loper < _MinNrSections;loper++)
        {
            ushort id = (ushort) loper;
            int length = _Default[loper]->getLength();
            int index = (length > SCPSection::Size ? sum : 0);
            pointers->setPointer(loper, id, length, index);
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
		SCPSection1* section1 = dynamic_cast<SCPSection1*>(_Default[1]);

        // Determine file Protocol Compatibility Level.
        if (pointers->Works()
                &&  (section1 != null)
                &&  (section1->Works()))
        {
			SCPSection2* section2 = dynamic_cast<SCPSection2*>(_Default[2]);
			SCPSection3* section3 = dynamic_cast<SCPSection3*>(_Default[3]);
			SCPSection7* section7 = dynamic_cast<SCPSection7*>(_Default[7]);
			SCPSection8* section8 = dynamic_cast<SCPSection8*>(_Default[8]);
            if ((section2 != null)
                &&  (section2->Works())
                &&  (section3 != null)
                &&  (section3->Works()))
            {
				SCPSection4* section4 = dynamic_cast<SCPSection4*>(_Default[4]);
				SCPSection5* section5 = dynamic_cast<SCPSection5*>(_Default[5]);
				SCPSection6* section6 = dynamic_cast<SCPSection6*>(_Default[6]);
                if ((section4 != null)
                		&&  (section4->Works())
                        &&  (section5 != null)
                		&&  (section5->Works())
                        &&  (section6 != null)
                		&&  (section6->Works()))
                {
                    section1->setProtocolCompatibilityLevel(SCPSection1::kProtocolCompatibilityCatIV);
                }
                else if ((section5 != null)
                		&&  (section5->Works()))
                {
                    section1->setProtocolCompatibilityLevel(SCPSection1::kProtocolCompatibilityCatIII);
                }
                else if ((section6 != null)
                		&&  (section6->Works()))
                {
                    section1->setProtocolCompatibilityLevel(SCPSection1::kProtocolCompatibilityCatII);
                }
                else if ((section7 != null)
                		&&  (section7->Works())
                        &&  (section8 != null)
                		&&  (section8->Works()))
                {
                    section1->setProtocolCompatibilityLevel(SCPSection1::kProtocolCompatibilityCatI);
                }
            }
            else if ((section7 != null)
                		&&  (section7->Works())
                        &&  (section8 != null)
                		&&  (section8->Works()))
            {
                section1->setProtocolCompatibilityLevel(SCPSection1::kProtocolCompatibilityCatI);
            }
        }
    }
}
/// <summary>
/// Function to empty entire format.
/// </summary>
void SCPFormat::EmptyFormat()
{
    for (int loper=0;loper < _MinNrSections;loper++)
    {
        _Default[loper]->Empty();
    }
//    _Manufactor = null;
}

//region IDisposable Members
void SCPFormat::Dispose()
{
    //			base.Dispose();

    _CRC = 0;
    _Length = 0;
    if (_Default.size() > 0)
    {
        for (int loper=0;loper < _Default.size();loper++)
        {
            if (_Default[loper] != null)
            {
                _Default[loper]->Empty();
                _Default[loper] = null;
            }
        }
        _Default.clear();
    }
}

#if 0//todo
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
#endif
}
}
