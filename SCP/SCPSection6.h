#ifndef _SCPSECTION6_H_
#define _SCPSECTION6_H_
#include "SCPSection.h"


namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 6 (Rhythm data section).
/// </summary>
class SCPSection6 : public SCPSection
{
    // Defined in SCP.
public:
    ushort getSectionID();
    bool Works();

    /// <summary>
    /// Function to set nr of leads used in section (Solution for a tiny problem).
    /// </summary>
    /// <param name="nrleads">nr of leads in section</param>
    void setNrLeads(ushort nrleads);

    /// <summary>
    /// Function to encode data in this section.
    /// </summary>
    /// <param name="data">Rhythm data to encode</param>
    /// <param name="tables">Huffman table to use during enconding</param>
    /// <param name="leadDefinition">Lead Definitions to use for encoding</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>0 on succes</returns>
    int EncodeData(short[][] data, SCPSection2 tables, SCPSection3 leadDefinition, SCPSection4 qrsLocations, int medianFreq, byte difference);

    /// <summary>
    /// Function to get AVM.
    /// </summary>
    /// <returns>AVM in uV</returns>
    double getAVM();

    /// <summary>
    /// Function to set AVM.
    /// </summary>
    /// <param name="avm">AVM in uV</param>
    void setAVM(double avm);

    /// <summary>
    /// Function to get samples per second used in data.
    /// </summary>
    /// <returns>samples per second</returns>
    int getSamplesPerSecond();

    /// <summary>
    /// Function to set samples per second used in data.
    /// </summary>
    /// <param name="sps">samples per second</param>
    void setSamplesPerSecond(int sps);

    /// <summary>
    /// Function to get bimodal settings.
    /// </summary>
    /// <returns>true if bimodal used</returns>
    bool getBimodal();

    /// <summary>
    /// Function to set bimodal settings.
    /// </summary>
    /// <param name="bimodal">true if bimodal used</param>
    void setBimodal(bool bimodal);
protected:
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    static ushort _SectionID;

    // special variable for setting nr leads before a read.
    ushort _NrLeads;

    // Part of the stored Data Structure.
    ushort _AVM;
    ushort _TimeInterval;
    byte _Difference;
    byte _Bimodal;
    ushort[] _DataLength;
    byte[][] _Data;
};
}
}
