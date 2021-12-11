#ifndef _SCPSECTION5_H_
#define _SCPSECTION5_H_
#include "SCPSection.h"
#include "SCPSection2.h"

namespace ECGConversion
{
namespace SCP
{
/// <summary>
/// Class contains section 5 (Reference beat data section).
/// </summary>
class SCPSection5 : SCPSection
{
public:
    SCPSection5();
    ~SCPSection5();
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
    /// <param name="medianLength">contains length of median data in msec</param>
    /// <param name="difference">difference to use durring decoding</param>
    /// <returns>0 on succes</returns>
    int EncodeData(short* dataArray,    					
						int nrleads, 
	    				int dataSingleLength, 
	    				SCPSection2* tables, 
	    				ushort medianLength, 
	    				uchar difference);
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
    int _Write(uchar* buffer, int bufferLength,int offset);
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
    uchar _Difference;
    uchar _Reserved;
    std::vector<ushort> _DataLength;
    std::vector<uchar*> _Data;
    std::vector<ushort> _DataRealLength;	
};
}
}

#endif  /*#ifndef _SCPSECTION5_H_*/
