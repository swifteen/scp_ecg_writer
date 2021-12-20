#include "SCPSection10.h"
#include "CRCTool.h"
#include "BytesTool.h"
#include "LeadType.h"
#include "MeasurementType.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion::ECGLeadMeasurements;
using namespace ECGConversion::ECGSignals;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// class for Lead Measurements from SCP.
/// </summary>
class SCPSection10::SCPLeadMeasurements
{
public:
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    SCPLeadMeasurements()
    {
        _LeadId = LeadTypeUnknown;
        _LeadLength = 0;
        _Measurements = null;
        _MeasurementsLength = 0;
    }
    /// <summary>
    /// Constructor to make a SCP statement.
    /// </summary>
    SCPLeadMeasurements(LeadType lt, ushort nrMes)
    {
        _LeadId = lt;
        setCount(nrMes);
        _Measurements = null;
        _MeasurementsLength = 0;
    }

    void setLeadType(LeadType LeadId)
    {
        _LeadId = (ushort) LeadId;
    }
    LeadType getLeadType()
    {
        return (LeadType)_LeadId;
    }

    void setCount(int Count)
    {
        int temp = (Count < 50) ? 50 : (Count << 1);

        if ((temp <= ushort_MaxValue)
            && (temp >= ushort_MinValue)) {
            _LeadLength = (ushort) temp;
            setLeadLength(_LeadLength);
        }
    }
    int getCount()
    {
        return (_Measurements == null ? 0 : _MeasurementsLength);
    }

    short getMeasurement(MeasurementType mt)
    {
        int id = (int)mt;

        if ((_MeasurementsLength == 0)
            || (id < 0)
            || (id >= _MeasurementsLength)) {
            return 0;
        }

        return _Measurements[id];
    }
    void setMeasurement(MeasurementType mt, short measurementValue)
    {
        int id = (int)mt;

        if ((_Measurements != null)
            && (id >= 0)
            && (id < _MeasurementsLength)) {
            _Measurements[id] = measurementValue;
        }
    }

    void setLeadLength(ushort LeadLength)
    {
        if (LeadLength >> 1 < 50) {
            LeadLength = 50 << 1;
        }

        if ((LeadLength == 0) && ((LeadLength & 0x1) != 0x1)) {
            return;
        }
        else {
            _MeasurementsLength = LeadLength >> 1;

            if (_Measurements != null) {
                delete [] _Measurements;
                _Measurements = null;
            }

            _Measurements = new short[_MeasurementsLength];

            if (_Measurements != null) {
                for (int i = 0; i < _MeasurementsLength; i++) {
                    MeasurementType mt = (MeasurementType) i;
                    bool bZero = (mt == MeasurementTypePmorphology)
                                 || (mt == MeasurementTypeTmorphology)
                                 || (mt == MeasurementTypeQualityCode)
                                 || (mt > MeasurementTypeSTamp1_8RR);
                    _Measurements[i] = bZero ? (short) 0 : LeadMeasurement::NoValue;
                }
            }
        }
    }

    /// <summary>
    /// Function to write SCP statement.
    /// </summary>
    /// <param name="buffer">byte array to write into</param>
    /// <param name="offset">position to start writing</param>
    /// <returns>0 on success</returns>
    int Write(uchar* buffer, int bufferLength, int offset)
    {
        if (!Works()) {
            SCP_PE("not Works\n");
            return 0x1;
        }

        if ((offset + getLength()) > bufferLength) {
            SCP_PE("out of range,offset: %d,getLength(): %d,bufferLength: %d\n", offset, getLength(), bufferLength);
            return 0x2;
        }

        int fieldSize = sizeof(_LeadId);
        BytesTool::writeBytes(_LeadId, buffer, bufferLength, offset, fieldSize, true);
        offset += fieldSize;
        fieldSize = sizeof(_LeadLength);
        BytesTool::writeBytes(_LeadLength, buffer, bufferLength, offset, fieldSize, true);
        offset += fieldSize;

        if (_LeadLength != 0) {
            fieldSize = sizeof(short);

            for (int i = 0; i < _MeasurementsLength; i++) {
                BytesTool::writeBytes(_Measurements[i], buffer, bufferLength, offset, fieldSize, true);
                offset += fieldSize;
            }
        }

        return 0x0;
    }
    /// <summary>
    /// Function to get length of SCP statement.
    /// </summary>
    /// <returns>length of statement</returns>
    int getLength()
    {
        if (Works()) {
            int sum = sizeof(_LeadId) + sizeof(_LeadLength);

            if (_Measurements != null) {
                sum += _LeadLength;
            }

            return sum;
        }

        return 0;
    }
    bool Works()
    {
        return _Measurements != null;
    }
private:
    short* _Measurements;
    int _MeasurementsLength;
    ushort _LeadId;
    ushort _LeadLength;
};

// Defined in SCP.
ushort SCPSection10::_SectionID = 10;
/// <summary>
/// Class contains section 10 (contains the Lead Measurements Result section).
/// </summary>
SCPSection10::SCPSection10()
{
    SCPSection::Empty();
}

ushort SCPSection10::getNrLeads()
{
    return (ushort)(_LeadMeasurements.size());
}

void SCPSection10::setNrLeads(ushort NrLeads)
{
    (NrLeads == 0) ? _LeadMeasurements.clear() : _LeadMeasurements.resize(NrLeads);
}

int SCPSection10::_Write(uchar* buffer, int bufferLength, int offset)
{
    if (!Works()) {
        return 0x1;
    }

    if ((offset + _getLength()) > bufferLength) {
        return 0x2;
    }

    if (_NrLeads  == 0) {
        return 0;
    }

    int fieldSize = sizeof(_NrLeads);
    BytesTool::writeBytes(_NrLeads, buffer, bufferLength, offset, fieldSize, true);
    offset += fieldSize;
    fieldSize = sizeof(_ManufactorSpecific);
    BytesTool::writeBytes(_ManufactorSpecific, buffer, bufferLength, offset, fieldSize, true);
    offset += fieldSize;

    if (_NrLeads == 0) {
        return 0x0;
    }

    for (int loper = 0; loper < _LeadMeasurements.size(); loper++) {
        int ret = _LeadMeasurements[loper].Write(buffer, bufferLength, offset);

        if (ret != 0) {
            return 0x1;
        }

        offset += _LeadMeasurements[loper].getLength();
    }

    return 0x0;
}
void SCPSection10::_Empty()
{
    _NrLeads = 0;
    _ManufactorSpecific = 0;
}
int SCPSection10::_getLength()
{
    if (Works()) {
        if (_NrLeads == 0) {
            return 0;
        }

        int sum = sizeof(_NrLeads) + sizeof(_ManufactorSpecific);

        if (_NrLeads != 0) {
            for (int loper = 0; loper < _LeadMeasurements.size(); loper++) {
                sum += _LeadMeasurements[loper].getLength();
            }
        }

        return ((sum % 2) == 0 ? sum : sum + 1);
    }

    return 0;
}
ushort SCPSection10::getSectionID()
{
    return _SectionID;
}
bool SCPSection10::Works()
{
    if (_NrLeads == 0) {
        return true;
    }

    for (int loper = 0; loper < _LeadMeasurements.size(); loper++) {
        if (!_LeadMeasurements[loper].Works()) {
            return false;
        }
    }

    return true;
}

int SCPSection10::setLeadMeasurements(LeadMeasurements& mes)
{
    int nrLeads = mes.Measurements.size();
    _NrLeads = (ushort) nrLeads;
    _LeadMeasurements.resize(nrLeads);

    for (int i = 0; i < nrLeads; i++) {
        int nrValues = mes.Measurements[i].getMeasurementCount();
        //        nrValues = (nrLeads > 0) ? ((int) mes.Measurements[i].getKeyByIndex(nrValues-1))+1 : 0;
        _LeadMeasurements[i].setLeadType(mes.Measurements[i].leadType);
        _LeadMeasurements[i].setCount(MeasurementTypeSum);
        nrValues = mes.Measurements[i].getMeasurementCount();

        for (int j = 0; j < (int)MeasurementTypeSum; j++) { //每个导联的测量矩阵
            if (mes.Measurements[i].getMeasurementValid((MeasurementType) i)) {
                _LeadMeasurements[i].setMeasurement((MeasurementType)j,
                                                    mes.Measurements[i].getMeasurement((MeasurementType)j));
            }
        }
    }

    return 0;
}
}
}
