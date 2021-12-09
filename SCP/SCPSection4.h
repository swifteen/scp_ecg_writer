#ifndef _SCPSECTION4_H_
#define _SCPSECTION4_H_



namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 4 (QRS locations).
/// </summary>
class SCPSection4 : public SCPSection, public ISignal
{
public:
    ushort getSectionID();

    bool Works();

    /// <summary>
    /// Function to get length of median.
    /// </summary>
    /// <returns>length of median data.</returns>
    ushort getMedianLength();

    /// <summary>
    /// Function to add median data to residual data.
    /// </summary>
    /// <remarks>both signals must have the same sample rate and AVM</remarks>
    /// <param name="definition">Data structure containing information about length of residual data.</param>
    /// <param name="residual">2D array containing residual data for each lead. On succes will contain rhythm data.</param>
    /// <param name="median">2D array containing median data for each lead.</param>
    /// <returns>error:
    /// 0x001) given data makes no sense.
    /// 0x002) Fault in Lead nr 0.
    /// 0x004) Fault in Lead nr 1.
    /// 0x008) Fault in Lead nr 2.
    /// 0x010) Fault in Lead nr 3.
    /// 0x020) Fault in Lead nr 4.
    /// 0x040) Fault in Lead nr 5.
    /// 0x080) Fault in Lead nr 6.
    /// 0x100) Fault in Lead nr 7.
    /// ...</returns>
    int AddMedians(SCPSection3 definition, short[][] residual, short[][] median);

    /// <summary>
    /// Function to subtract median data to residual data.
    /// </summary>
    /// <remarks>both signals must have the same sample rate and AVM</remarks>
    /// <param name="definition">Data structure containing information about length of rhythm data.</param>
    /// <param name="rhythm">2D array containing rhythm data for each lead. On succes will contain residual data.</param>
    /// <param name="median">2D array containing median data for each lead.</param>
    /// <returns>error:
    /// 0x001) given data makes no sense.
    /// 0x002) Fault in Lead nr 0.
    /// 0x004) Fault in Lead nr 1.
    /// 0x008) Fault in Lead nr 2.
    /// 0x010) Fault in Lead nr 3.
    /// 0x020) Fault in Lead nr 4.
    /// 0x040) Fault in Lead nr 5.
    /// 0x080) Fault in Lead nr 6.
    /// 0x100) Fault in Lead nr 7.
    /// ...</returns>
    int SubtractMedians(SCPSection3 definition, short[][] rhythm, short[][] median);

    /// <summary>
    /// Function to get nr of protected zones.
    /// </summary>
    /// <returns>nr of protected zones</returns>
    int getNrProtectedZones();

    /// <summary>
    /// Function to get start of protected zone.
    /// </summary>
    /// <param name="nr">nr of protected zone</param>
    /// <returns>start sample nr of protected zone</returns>
    int getProtectedStart(int nr);

    /// <summary>
    /// Function to get end of protected zone.
    /// </summary>
    /// <param name="nr">nr of protected zone</param>
    /// <returns>end sample nr of protected zone</returns>
    int getProtectedEnd(int nr);

    /// <summary>
    /// Function to get length of protected zone.
    /// </summary>
    /// <param name="nr">nr of protected zone</param>
    /// <returns>length of protected zone in samples</returns>
    int getProtectedLength(int nr);

    /// <summary>
    /// Function to set protected zones using global measurements.
    /// </summary>
    /// <param name="global">global measurments</param>
    /// <param name="medianFreq">Samples per Second of median</param>
    /// <param name="rate">Bimodal compression rate</param>
    /// <param name="minbegin">Begin of all leads in samples</param>
    /// <param name="maxend">End of all leads in samples</param>
    void setProtected(GlobalMeasurements global, int medianFreq, int rate, int minbegin, int maxend);
    // Signal Manupalations
    int getSignals(out Signals signals);
    int getSignalsToObj(Signals signals);
    int setSignals(Signals signals);
protected:
    int _Read(byte[] buffer, int offset);
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static ushort _SectionID;

    // Part of the stored Data Structure.
    ushort _MedianDataLength;
    ushort _FirstFiducial;
    ushort _NrQRS;
    class SCPQRSSubtraction;
    SCPQRSSubtraction[] _Subtraction;
    class SCPQRSProtected;
    SCPQRSProtected[] _Protected;
};
}

#endif  /*#ifndef _SCPSECTION4_H_*/
