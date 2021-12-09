#include "SCPSection1.h"
#include "CRCTool.h"
#include "BytesTool.h"

using namespace Communication_IO_Tools;
using namespace ECGConversion.ECGDemographics;

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class for a header field.
/// </summary>
class SCPSection1::SCPHeaderField
{
    /// <summary>
    /// Constructor to make an SCP header field.
    /// </summary>
    SCPHeaderField()
    {
        Tag = 0;
        Length = 0;
        Value = null;
    }
    /// <summary>
    /// Constructor to make an SCP header field.
    /// </summary>
    /// <param name="tag">tag to use</param>
    /// <param name="length">length to use</param>
    /// <param name="value">array to use</param>
    SCPHeaderField(byte tag, ushort length, byte[] value)
    {
        Tag = tag;
        Length = (value == null ? (ushort)0 : length);
        Value = value;
    }
private:
    byte Tag;
    ushort Length;
    byte[] Value;
}

// Fields that must be made empty for anonymous. (must be sorted from low to high)
byte[] SCPSection1::_AnonymousFields = {0, 1, 2, 3, 5, 30, 31};
// Defined in SCP.
byte[] SCPSection1::_MustBePresent = {2, 14, 25, 26}; // defined in paragraph 5.4.3.1 of SCP
byte[] SCPSection1::_MultipleInstanceFields = {10, 13, 30, 32, 35}; // Must be sorted
ushort SCPSection1::_MaximumFieldLength = 64;
byte[] SCPSection1::_MaximumLengthExceptions = {13, 14, 15, 30, 35}; // Must be sorted
ushort SCPSection1::_ExceptionsMaximumLength = 256; // should be 80, but some scp file doen't use this maximum. apparantly 128 wasn't enough as well
byte SCPSection1::_ManufactorField = 0xc8;
ushort SCPSection1::_SectionID = 1;
byte SCPSection1::_DemographicTerminator = 0xff;

// ResizeSpeed for the array to store the Fields.
int SCPSection1::_ResizeSpeed = 8;

/// <summary>
/// Class contains section 1 (Header Information).
/// </summary>
SCPSection1::SCPSection1()
{
    // Part of the stored Data Structure.
    _NrFields = 0;
    _Fields = null;
}

int SCPSection1::_Write(byte[] buffer, int offset)
{
    for (int loper=0;loper < _NrFields;loper++)
    {
        BytesTool::writeBytes(_Fields[loper].Tag , buffer, offset, sizeof(_Fields[loper].Tag), true);
        offset += sizeof(_Fields[loper].Tag);
        BytesTool::writeBytes(_Fields[loper].Length , buffer, offset, sizeof(_Fields[loper].Length), true);
        offset += sizeof(_Fields[loper].Length);
        offset += BytesTool::copy(buffer, offset, _Fields[loper].Value, 0, _Fields[loper].Length);
    }
    return 0x0;
}

void SCPSection1::_Empty()
{
    _NrFields = 0;
    _Fields = null;
}

ushort SCPSection1::getSectionID()
{
    return _SectionID;
}

int SCPSection1::_getLength()
{
    if (Works())
    {
        int sum = 0;
        for (int loper=0;loper < _NrFields;loper++)
        {
            sum += (sizeof(_Fields[loper].Tag) + sizeof(_Fields[loper].Length) + _Fields[loper].Length);
        }
        return ((sum % 2) == 0 ? sum : sum + 1);
    }
    return 0;
}

bool SCPSection1::Works()
{
    if (CheckInstances())
    {
        for (int loper=0;loper < _NrFields;loper++)
        {
            if ((_Fields[loper] == null)
                    ||  ((_Fields[loper].Value == null) && (_Fields[loper].Length != 0))
                    ||  ((_Fields[loper].Value != null) && (_Fields[loper].Value.Length != _Fields[loper].Length))
                    ||  ((_Fields[loper].Length > _MaximumFieldLength) && (_Fields[loper].Tag < _ManufactorField) && (!isException(_MaximumLengthExceptions, _Fields[loper].Tag) || (_Fields[loper].Length > _ExceptionsMaximumLength))))
            {
                return false;
            }
        }
        for (int loper=0;loper < _MustBePresent.Length;loper++)
        {
            if (_SearchField(_MustBePresent[loper]) < 0)
            {
                return false;
            }
        }
        return (_Fields[_NrFields - 1].Tag == 0xff) &&  (_Fields[_NrFields - 1].Length == 0);
    }
    return false;
}

/// <summary>
/// Function to initialize a section 1. Only needed when not reading from buffer.
/// </summary>
void SCPSection1::Init()
{
    _Empty();
    _Fields = new SCPHeaderField[_ResizeSpeed];
    _Fields[_NrFields++] = new SCPHeaderField(_DemographicTerminator, 0, null);
}

/// <summary>
/// Function to insert a field into section.
/// </summary>
/// <param name="field">field to insert</param>
/// <returns>0 on success</returns>
int SCPSection1::Insert(SCPHeaderField field)
{
    if ((field != null)
            &&  (field.Tag != _DemographicTerminator)
            &&	(_Fields != null)
            &&  (_NrFields <= _Fields.Length)
            &&  (_Fields[_NrFields - 1].Tag == _DemographicTerminator))
    {
        if ((field.Length == 0)
                ||	((field.Value != null)
                     &&	 (field.Length <= field.Value.Length)))
        {
            int p1 = _SearchField(field.Tag);
            // If field exist must override or can be an multiple instances.
            if (p1 >= 0)
            {
                // If multiple instaces field, add field as last of this kind of field.
                if (isException(_MultipleInstanceFields, field.Tag))
                {
                    // Resize if space is needed.
                    if (_NrFields == _Fields.Length)
                    {
                        Resize();
                    }
                    // Find last of this kind.
                    for (;(p1 < _NrFields) && (_Fields[p1].Tag == field.Tag);p1++);
                    // Make space in array for field.
                    for (int loper = _NrFields;loper > p1;loper--)
                    {
                        _Fields[loper] = _Fields[loper - 1];
                    }
                    _Fields[p1] = field;
                    _NrFields++;
                }
                else
                {
                    // Overwrite existing field.
                    _Fields[p1] = field;
                }
            }
            else
            {
                // Resize if space is needed
                if (_NrFields == _Fields.Length)
                {
                    Resize();
                }
                int p2 = _InsertSearch(field.Tag);
                // Make space to insert.
                for (int loper = _NrFields;loper > p2;loper--)
                {
                    _Fields[loper] = _Fields[loper - 1];
                }
                _Fields[p2] = field;
                _NrFields++;
            }
            return 0x0;
        }
        return 0x2;
    }
    return 0x1;
}

/// <summary>
/// Function to remove a certain field from section.
/// </summary>
/// <param name="tag">tag of field.</param>
/// <returns>0 on success</returns>
int SCPSection1::Remove(byte tag)
{
    if ((tag != _DemographicTerminator)
            &&  (_Fields != null)
            &&  (_NrFields <= _Fields.Length))
    {
        int p = _SearchField(tag);
        if (p >= 0)
        {
            _NrFields--;
            for (;p < _NrFields;p++)
            {
                _Fields[p] = _Fields[p+1];
            }
            return 0x0;
        }
        return 0x2;
    }
    return 0x1;
}

/// <summary>
/// Function to resize the space for header fields.
/// </summary>
void SCPSection1::Resize()
{
    SCPHeaderField[] temp = new SCPHeaderField[_NrFields + _ResizeSpeed];
    for (int loper=0;loper < _NrFields;loper++)
    {
        temp[loper] = _Fields[loper];
    }
    _Fields = temp;
}

/// <summary>
/// Function to get a field from this section
/// </summary>
/// <param name="tag">tag to search for</param>
/// <returns></returns>
SCPHeaderField SCPSection1::GetField(byte tag)
{
    int pos = _SearchField(tag);

    if ((_Fields != null)
            &&	(pos >= 0)
            &&	(pos < _Fields.Length))
    {
        return _Fields[pos];
    }

    return null;
}

/// <summary>
/// Function to search for a field with a certain tag.
/// </summary>
/// <param name="tag">tag to search for</param>
/// <returns>position of this field</returns>
int SCPSection1::_SearchField(byte tag)
{
    int l = 0;
    int h = _NrFields-1;
    int m = (h >> 1);
    while (l <= h && _Fields[m].Tag != tag)
    {
        if (tag > _Fields[m].Tag)
        {
            l = m + 1;
        }
        else
        {
            h = m - 1;
        }
        m = ((l + h) >> 1);
    }
    if ((m >= 0) && (m < _NrFields) && (_Fields[m].Tag == tag))
    {
        return m;
    }
    return -1;
}

/// <summary>
/// Function to find position to insert a field with a certain tag.
/// </summary>
/// <param name="tag">tag to search on.</param>
/// <returns>position to insert</returns>
int SCPSection1::_InsertSearch(byte tag)
{
    int l = 0, h = _NrFields;
    while (l < h)
    {
        int m = (l + h) / 2;
        if (_Fields[m].Tag < tag)
            l = m + 1;
        else
            h = m;
    }
    return l;
}

/// <summary>
/// Function to check wheter the used fields are indeed sorted.
/// </summary>
bool SCPSection1::CheckInstances()
{
    if ((_Fields != null)
            &&  (_NrFields > 0)
            &&  (_NrFields <= _Fields.Length)
            &&  (_Fields[0] != null))
    {
        byte prev = _Fields[0].Tag;
        for (int loper=1;loper < _NrFields;loper++)
        {
            if ((prev == _Fields[loper].Tag)
                    &&  !isException(_MultipleInstanceFields, prev))
            {
                return false;
            }
            prev = _Fields[loper].Tag;
        }
        return true;
    }
    return false;
}

/// <summary>
/// Function to check for exception case.
/// </summary>
/// <param name="condition">condition</param>
/// <param name="tag">value of tag</param>
/// <returns>is exception then true</returns>
static bool SCPSection1::isException(byte[] condition, byte tag)
{
    if (condition == null)
    {
        return false;
    }
    int l = 0;
    int h = condition.Length - 1;
    int m = (h >> 1);
    while (l <= h && condition[m] != tag)
    {
        if (tag > condition[m])
        {
            l = m + 1;
        }
        else
        {
            h = m - 1;
        }
        m = ((l + h) >> 1);
    }
    return (m >= 0) && (m < condition.Length) && (condition[m] == tag);
}

#if 0
/// <summary>
/// Get encoding for text from language support code.
/// </summary>
/// <returns>used encoding</returns>
System.Text.Encoding SCPSection1::getLanguageSupportCode()
{
    System.Text.Encoding enc;

    getLanguageSupportCode(out enc);

    return enc;
}

/// <summary>
/// Get encoding for text from language support code.
/// </summary>
/// <param name="enc">used encoding</param>
/// <returns>0 if successfull</returns>
int SCPSection1::getLanguageSupportCode(out System.Text.Encoding enc)
{
    enc = System.Text.Encoding.Default;

    int p = _SearchField(14);
    if ((p >= 0)
            &&	(_Fields[p] != null)
            &&  (_Fields[p].Value != null)
            &&  (_Fields[p].Length <= _Fields[p].Value.Length)
            &&  (_Fields[p].Length > 16))
    {
        byte lsc = _Fields[p].Value[16];

        if ((lsc & 0x1) == 0x0)
        {
            return 0;
        }
        else if ((lsc & 0x3) == 0x1)
        {
            enc = System.Text.Encoding.GetEncoding("ISO-8859-1");

            return 0;
        }
        else
        {
            string encName = null;

            switch (lsc)
            {
            case 0x03: encName = "ISO-8859-2"; break;
            case 0x0b: encName = "ISO-8859-4"; break;
            case 0x13: encName = "ISO-8859-5"; break;
            case 0x1b: encName = "ISO-8859-6"; break;
            case 0x23: encName = "ISO-8859-7"; break;
            case 0x2b: encName = "ISO-8859-8"; break;
            case 0x33: encName = "ISO-8859-11"; break;
            case 0x3b: encName = "ISO-8859-15"; break;
            case 0x07: encName = "utf-16"; break; //case 0x07: encName = "ISO-60646"; break;
            case 0x0f: //encName = "JIS X0201-1976";
            case 0x17: //encName = "JIS X0208-1997";
            case 0x1f: //encName = "JIS X0212-1990";
                encName = "EUC-JP";
                break;
            case 0x27: encName = "gb2312"; break;
            case 0x2f: encName = "ks_c_5601-1987"; break;
            default: break;
            }

            if (encName != null)
            {
                enc = System.Text.Encoding.GetEncoding(encName);

                return 0;
            }
        }
    }
    return 1;
}

/// <summary>
/// Set language support code based on encoding.
/// </summary>
/// <param name="enc">encoding to set lsc with.</param>
/// <returns>0 if successfull</returns>
int SCPSection1::setLanguageSupportCode(System.Text.Encoding enc)
{
    int ret = 0;
    byte lsc = 0;

    switch (enc.CodePage)
    {
    case 20127: break;
    case 28591: lsc = 0x01; break;
    case 28592: lsc = 0x03; break;
    case 28594: lsc = 0x0b; break;
    case 28595: lsc = 0x13; break;
    case 28596: lsc = 0x1b; break;
    case 28597: lsc = 0x23; break;
    case 28598: lsc = 0x2b; break;
    case 28603: lsc = 0x33; break;
    case 28605: lsc = 0x3b; break;
    case  1200: lsc = 0x07; break;
    case 20932: lsc = 0x1f; break;
    case 20936: lsc = 0x27; break;
    case   949: lsc = 0x2f; break;
    default: ret = 1; break;
    }

    int p = _SearchField(14);
    if ((p >= 0)
            &&	(_Fields[p] != null)
            &&  (_Fields[p].Value != null)
            &&  (_Fields[p].Length <= _Fields[p].Value.Length)
            &&  (_Fields[p].Length > 16))
    {
        _Fields[p].Value[16] = lsc;
    }
    else if (ret != 1)
    {
        ret = 2;
    }

    p = _SearchField(15);
    if ((p >= 0)
            &&	(_Fields[p] != null)
            &&  (_Fields[p].Value != null)
            &&  (_Fields[p].Length <= _Fields[p].Value.Length)
            &&  (_Fields[p].Length > 16))
    {
        _Fields[p].Value[16] = lsc;
    }


    return ret;
}
#endif

/// <summary>
/// Function to get Protocol Compatability Level.
/// </summary>
/// <param name="pc">Protocol Compatability Level</param>
/// <returns>0 on succes</returns>
int SCPSection1::getProtocolCompatibilityLevel(out ProtocolCompatibility pc)
{
    pc = 0;
    int p = _SearchField(14);
    if ((p >= 0)
            &&	(_Fields[p] != null)
            &&  (_Fields[p].Value != null)
            &&  (_Fields[p].Length <= _Fields[p].Value.Length)
            &&  (_Fields[p].Length > 15))
    {
        pc = (ProtocolCompatibility) _Fields[p].Value[15];
        return 0;
    }
    return 1;
}

/// <summary>
/// Function to set Protocol Compatability Level.
/// </summary>
/// <param name="pc">Protocol Compatability Level</param>
/// <returns>0 on succes</returns>
int SCPSection1::setProtocolCompatibilityLevel(ProtocolCompatibility pc)
{
    int p = _SearchField(14);
    if ((p >= 0)
            &&	(_Fields[p] != null)
            &&  (_Fields[p].Value != null)
            &&  (_Fields[p].Length <= _Fields[p].Value.Length)
            &&  (_Fields[p].Length > 15))
    {
        _Fields[p].Value[15] = (byte) pc;
        return 0;
    }
    return 1;
}

/// <summary>
/// Function to set a text from a cetain tag.
/// </summary>
/// <param name="tag">id of tag</param>
/// <param name="text">a string</param>
/// <returns>0 on success</returns>
int SCPSection1::setText(byte tag, string text)
{
    if (text != null)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = tag;
        field.Length = (ushort) (text.Length >= _MaximumFieldLength ? _MaximumFieldLength :  text.Length + 1);
        field.Value = new byte[field.Length];
        BytesTool::writeString(_Encoding, text, field.Value, 0, field.Length);
        return Insert(field) << 1;
    }
    return 1;
}

// Getting Demographics information
void SCPSection1::setLastName(const string& value)
{
    setText(0, value);
}

void SCPSection1::setFirstName(const string& value)
{
    setText(1, value);
}

void SCPSection1::setPatientID(const string& value)
{
    setText(2, value);
}

void SCPSection1::setSecondLastName(const string& value)
{
    setText(3, value);
}

int SCPSection1::setPatientAge(ushort val, AgeDefinition def)
{
    SCPHeaderField field = new SCPHeaderField();
    field.Tag = 4;
    field.Length = (ushort) (sizeof(val) + sizeof(typeof(byte)));
    field.Value = new byte[field.Length];
    BytesTool::writeBytes(val, field.Value, 0, sizeof(val), true);
    BytesTool::writeBytes((byte)def, field.Value, sizeof(val), sizeof(typeof(byte)), true);
    return Insert(field) << 1;
}

void SCPSection1::setPatientBirthDate(Date PatientBirthDate)
{
    if ((PatientBirthDate != null)
            &&	(PatientBirthDate.isExistingDate()))
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 5;
        field.Length = (ushort) SCPDate.Size;
        field.Value = new byte[field.Length];
        SCPDate scpdate = new SCPDate();
        scpdate.Year = PatientBirthDate.Year;
        scpdate.Month = PatientBirthDate.Month;
        scpdate.Day = PatientBirthDate.Day;
        scpdate.Write(field.Value, 0);

        Insert(field);
    }
}

int SCPSection1::setPatientHeight(ushort val, HeightDefinition def)
{
    SCPHeaderField field = new SCPHeaderField();
    field.Tag = 6;
    field.Length = (ushort) (sizeof(val) + sizeof(typeof(byte)));
    field.Value = new byte[field.Length];
    BytesTool::writeBytes(val, field.Value, 0, sizeof(val), true);
    BytesTool::writeBytes((byte)def, field.Value, sizeof(val), 1, true);
    return Insert(field) << 1;
}

int SCPSection1::setPatientWeight(ushort val, WeightDefinition def)
{
    SCPHeaderField field = new SCPHeaderField();
    field.Tag = 7;
    field.Length = (ushort) (sizeof(val) + sizeof(typeof(byte)));
    field.Value = new byte[field.Length];
    BytesTool::writeBytes(val, field.Value, 0, sizeof(val), true);
    BytesTool::writeBytes((byte)def, field.Value, sizeof(val), sizeof(typeof(byte)), true);
    return Insert(field) << 1;
}

void SCPSection1::setGender(Sex Gender)
{
    if (Gender != Sex.Null)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 8;
        field.Length = (ushort) sizeof(typeof(byte));
        field.Value = new byte[field.Length];
        BytesTool::writeBytes((byte)Gender, field.Value, 0, sizeof(typeof(byte)), true);
        Insert(field);
    }
}

void SCPSection1::setPatientRace(Race PatientRace)
{
    if (PatientRace != Race.Null)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 9;
        field.Length = (ushort) sizeof(typeof(byte));
        field.Value = new byte[field.Length];
        BytesTool::writeBytes((byte)PatientRace, field.Value, 0, sizeof(typeof(byte)), true);
        Insert(field);
    }
}

void SCPSection1::setAcqMachineID(AcquiringDeviceID AcqMachineID)
{
    AcquiringDeviceID id = value;

    if ((id != null)
            &&  (id.ModelDescription != null))
    {
        SCPHeaderField field = new SCPHeaderField();
        string deviceManufactor = (id.ManufactorID == 0 ? ECGConverter.SoftwareName : ((DeviceManufactor)id.ManufactorID).ToString());
        string unknown = "unknown";
        field.Tag = 14;
        field.Length = (ushort) (41 + (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + deviceManufactor.Length + (3 * unknown.Length));
        field.Value = new byte[field.Length];
        int offset = 0;
        BytesTool::writeBytes(id.InstitutionNr, field.Value, offset, sizeof(id.InstitutionNr), true);
        offset += sizeof(id.InstitutionNr);
        BytesTool::writeBytes(id.DepartmentNr, field.Value, offset, sizeof(id.DepartmentNr), true);
        offset += sizeof(id.DepartmentNr);
        BytesTool::writeBytes(id.DeviceID, field.Value, offset, sizeof(id.DeviceID), true);
        offset += sizeof(id.DeviceID);
        BytesTool::writeBytes(id.DeviceType, field.Value, offset, sizeof(id.DeviceType), true);
        offset += sizeof(id.DeviceType);
        BytesTool::writeBytes((id.ManufactorID == 0 ? (byte) 0xff : id.ManufactorID), field.Value, offset, sizeof(id.ManufactorID), true);
        offset += sizeof(id.ManufactorID);
        offset += BytesTool::copy(field.Value, offset, id.ModelDescription, 0, id.ModelDescription.Length);
        field.Value[offset++] = ProtocolVersionNr;
        field.Value[offset++] = 0x00;
        field.Value[offset++] = 0x00;
        field.Value[offset++] = (id.DeviceCapabilities == 0 ? (byte) 0x8 : id.DeviceCapabilities);
        field.Value[offset++] = id.ACFrequencyEnvironment;

        // Skip Reserved for Future field
        offset += 16;

        field.Value[offset++] = (byte) (unknown.Length + 1);

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, ECGConverter.SoftwareName, field.Value, offset, (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + 1);
        offset+= (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + 1;

        BytesTool::writeString(_Encoding, deviceManufactor, field.Value, offset, deviceManufactor.Length + 1);
        offset+= deviceManufactor.Length + 1;

        int ret = Insert(field);

        if (ret == 0)
            ret = setLanguageSupportCode(_Encoding);
    }
}

void SCPSection1::setAnalyzingMachineID(AcquiringDeviceID AnalyzingMachineID)
{
    AcquiringDeviceID id = AnalyzingMachineID;

    if ((id != null)
            &&  (id.ModelDescription != null))
    {
        SCPHeaderField field = new SCPHeaderField();
        string deviceManufactor = (id.ManufactorID == 0 ? ECGConverter.SoftwareName : ((DeviceManufactor)id.ManufactorID).ToString());
        string unknown = "unknown";
        field.Tag = 15;
        field.Length = (ushort) (41 + (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + deviceManufactor.Length + (3 * unknown.Length));
        field.Value = new byte[field.Length];
        int offset = 0;
        BytesTool::writeBytes(id.InstitutionNr, field.Value, offset, sizeof(id.InstitutionNr), true);
        offset += sizeof(id.InstitutionNr);
        BytesTool::writeBytes(id.DepartmentNr, field.Value, offset, sizeof(id.DepartmentNr), true);
        offset += sizeof(id.DepartmentNr);
        BytesTool::writeBytes(id.DeviceID, field.Value, offset, sizeof(id.DeviceID), true);
        offset += sizeof(id.DeviceID);
        BytesTool::writeBytes(id.DeviceType, field.Value, offset, sizeof(id.DeviceType), true);
        offset += sizeof(id.DeviceType);
        BytesTool::writeBytes(id.ManufactorID == 0 ? (byte)0xff : id.ManufactorID, field.Value, offset, sizeof(id.ManufactorID), true);
        offset += sizeof(id.ManufactorID);
        offset += BytesTool::copy(field.Value, offset, id.ModelDescription, 0, id.ModelDescription.Length);
        field.Value[offset++] = ProtocolVersionNr;
        field.Value[offset++] = 0x00;
        field.Value[offset++] = 0x00;
        field.Value[offset++] = (id.DeviceCapabilities == 0 ? (byte) 0x8 : id.DeviceCapabilities);
        field.Value[offset++] = id.ACFrequencyEnvironment;

        // Skip Reserved for Future field
        offset += 16;

        field.Value[offset++] = (byte) (unknown.Length + 1);

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, unknown, field.Value, offset, unknown.Length + 1);
        offset+= unknown.Length + 1;

        BytesTool::writeString(_Encoding, ECGConverter.SoftwareName, field.Value, offset, (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + 1);
        offset+= (ECGConverter.SoftwareName.Length > 24 ? 24 : ECGConverter.SoftwareName.Length) + 1;

        BytesTool::writeString(_Encoding, deviceManufactor, field.Value, offset, deviceManufactor.Length + 1);
        offset+= deviceManufactor.Length + 1;

        int ret = Insert(field);

        if (ret == 0)
            ret = setLanguageSupportCode(_Encoding);
    }
}

void SCPSection1::setTimeAcquisition(DateTime TimeAcquisition)
{
    DateTime time = value;

    if (time.Year > 1000)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 25;
        field.Length = (ushort) SCPDate.Size;
        field.Value = new byte[field.Length];
        SCPDate scpdate = new SCPDate();
        scpdate.Year = (ushort) time.Year;
        scpdate.Month = (byte) time.Month;
        scpdate.Day = (byte) time.Day;
        scpdate.Write(field.Value, 0);
        int ret = Insert(field);

        if (ret != 0)
            return;

        field = new SCPHeaderField();
        field.Tag = 26;
        field.Length = (ushort) SCPTime.Size;
        field.Value = new byte[field.Length];
        SCPTime scptime = new SCPTime();
        scptime.Hour = (byte) time.Hour;
        scptime.Min = (byte) time.Minute;
        scptime.Sec = (byte) time.Second;
        scptime.Write(field.Value, 0);

        Insert(field);
    }
}

void SCPSection1::setBaselineFilter(ushort BaselineFilter)
{
    SCPHeaderField field = new SCPHeaderField();
    field.Tag = 27;
    field.Length = (ushort) sizeof(value);
    field.Value = new byte[field.Length];
    BytesTool::writeBytes(value, field.Value, 0, sizeof(value), true);
    Insert(field);
}

void SCPSection1::setLowpassFilter(ushort LowpassFilter)
{
    SCPHeaderField field = new SCPHeaderField();
    field.Tag = 28;
    field.Length = (ushort) sizeof(value);
    field.Value = new byte[field.Length];
    BytesTool::writeBytes(value, field.Value, 0, sizeof(value), true);
    Insert(field);
}

void SCPSection1::setFilterBitmap(byte FilterBitmap)
{
    if (value != 0)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 29;
        field.Length = (ushort) sizeof(value);
        field.Value = new byte[field.Length];
        BytesTool::writeBytes(value, field.Value, 0, sizeof(value), true);
        Insert(field);
    }
}

void SCPSection1::setFreeTextFields(string[] FreeTextFields)
{
    if (value != null)
    {
        for (int loper=0;loper < value.Length;loper++)
        {
            if (value[loper] != null)
            {
                SCPHeaderField field = new SCPHeaderField();
                field.Tag = 30;
                field.Length = (ushort) (value[loper].Length >= _ExceptionsMaximumLength ? _ExceptionsMaximumLength : value[loper].Length + 1);
                field.Value = new byte[field.Length];
                BytesTool::writeString(_Encoding, value[loper], field.Value, 0, field.Length);
                Insert(field);
            }
        }
    }
}

void SCPSection1::setSequenceNr(string SequenceNr)
{
    setText(31, value);
}

void SCPSection1::setAcqInstitution(string AcqInstitution)
{
    setText(16, value);
}

void SCPSection1::setAnalyzingInstitution(string AnalyzingInstitution)
{
    setText(17, value);
}

void SCPSection1::setAcqDepartment(string AcqDepartment)
{
    setText(18, value);
}

void SCPSection1::setAnalyzingDepartment(string AnalyzingDepartment)
{
    setText(19, value);
}

void SCPSection1::setReferringPhysician(string ReferringPhysician)
{
    setText(20, value);
}

void SCPSection1::setOverreadingPhysician(string OverreadingPhysician)
{
    setText(21, value);
}

void SCPSection1::setTechnicianDescription(string TechnicianDescription)
{
    setText(22, value);
}

void SCPSection1::setSystolicBloodPressure(ushort SystolicBloodPressure)
{
    if (value != 0)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 11;
        field.Length = (ushort) sizeof(typeof(ushort));
        field.Value = new byte[field.Length];
        BytesTool::writeBytes(value, field.Value, 0, field.Length, true);
        Insert(field);
    }
}

void SCPSection1::setDiastolicBloodPressure(ushort DiastolicBloodPressure)
{
    if (value != 0)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 12;
        field.Length = (ushort) sizeof(typeof(ushort));
        field.Value = new byte[field.Length];
        BytesTool::writeBytes(value, field.Value, 0, field.Length, true);
        Insert(field);
    }
}

void SCPSection1::setDrugs(std::vector<Drug> Drugs)
{
    if (value != null)
    {
        for (int loper=0;loper < value.Length;loper++)
        {
            if (value[loper] != null)
            {
                SCPHeaderField field = new SCPHeaderField();
                field.Tag = 10;
                field.Length = (ushort) (4 + (value[loper].TextDesciption != null ? value[loper].TextDesciption.Length : 0));
                field.Value = new byte[field.Length];
                field.Value[0] = 0;
                field.Value[1] = value[loper].DrugClass;
                field.Value[2] = value[loper].ClassCode;
                BytesTool::writeString(_Encoding, value[loper].TextDesciption, field.Value, 3, field.Length - 3);

                Insert(field);
            }
        }
    }
}

void SCPSection1::setReferralIndication(string[] ReferralIndication)
{
    if (value != null)
    {
        for (int loper=0;loper < value.Length;loper++)
        {
            if (value[loper] != null)
            {
                SCPHeaderField field = new SCPHeaderField();
                field.Tag = 13;
                field.Length = (ushort) (value[loper].Length >= _ExceptionsMaximumLength ? _ExceptionsMaximumLength : value[loper].Length + 1);
                field.Value = new byte[field.Length];
                BytesTool::writeString(_Encoding, value[loper], field.Value, 0, field.Length);
                Insert(field);
            }
        }
    }
}

void SCPSection1::setRoomDescription(string RoomDescription)
{
    setText(23, value);
}

void SCPSection1::setStatCode(byte StatCode)
{
    if (value != 0xff)
    {
        SCPHeaderField field = new SCPHeaderField();
        field.Tag = 24;
        field.Length = (ushort) sizeof(value);
        field.Value = new byte[field.Length];
        BytesTool::writeBytes(value, field.Value, 0, sizeof(value), true);
        Insert(field);
    }
}

}
}
