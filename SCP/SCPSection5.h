#ifndef _ZQ_SCPSECTION5_H_
#define _ZQ_SCPSECTION5_H_


namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 5 (Reference beat data section).
/// </summary>
class SCPSection5 : SCPSection
{
public:
    SCPSection5();
    ushort getSectionID();
    bool Works();
    /// <summary>
    /// Function to set nr of leads used in section (Solution for a tiny problem).
    /// </summary>
    /// <param name="nrleads">nr of leads in section</param>
    void setNrLeads(ushort nrleads);
    /// <summary>
    /// Function to decode data in this section.
    /// </summary>
    /// <param name="tables">Huffman table to use during deconding</param>
    /// <param name="length">nr of samples in encoded data</param>
    /// <returns>decoded leads</returns>
    short[][] DecodeData(SCPSection2 tables, ushort length);

    /// <summary>
    /// Function to encode data in this section.
    /// </summary>
    /// <param name="data">Rhythm data to encode</param>
    /// <param name="tables">Huffman table to use during enconding</param>
    /// <param name="medianLength">contains length of median data in msec</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>0 on succes</returns>
    int EncodeData(short[][] data, SCPSection2 tables, ushort medianLength, byte difference);
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
protected:

    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // special variable for setting nr leads before a read.
    ushort _NrLeads;

    // Part of the stored Data Structure.
    ushort _AVM;
    ushort _TimeInterval;
    byte _Difference;
    byte _Reserved;
    ushort[] _DataLength;
    byte[][] _Data ;
};
}


#endif  /*#ifndef _ZQ_SCPSECTION5_H_*/
