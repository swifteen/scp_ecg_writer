#ifndef _SCPSECTION0_H_
#define _SCPSECTION0_H_
#include "SCPSection.h"


namespace ECGConversion.SCP
{
/// <summary>
/// Class contains section 0 (Pointer section).
/// </summary>
public class SCPSection0 : SCPSection
{
public:
    SCPSection0();
    ushort getSectionID();
    bool Works();
    /// <summary>
    /// Function to get number of pointers contained in pointer section.
    /// </summary>
    /// <returns>number of pointers</returns>
    int getNrPointers();
    /// <summary>
    /// Function to set number of pointers contained in pointer section.
    /// </summary>
    /// <param name="nr">number of pointers</param>
    void setNrPointers(int nr);
    /// <summary>
    /// Function to get section id of a pointer.
    /// </summary>
    /// <param name="nr">pointer number to get section id from</param>
    /// <returns>id of section</returns>
    ushort getSectionID(int nr);
    /// <summary>
    /// Function to set section id of a pointer.
    /// </summary>
    /// <param name="nr">number of pointer to set</param>
    /// <param name="ID">id of section</param>
    void setSectionID(int nr, ushort ID);
    /// <summary>
    /// Function to get index of a pointer.
    /// </summary>
    /// <param name="nr">number of pointer to get from</param>
    /// <returns>0 on success</returns>
    int getIndex(int nr);
    /// <summary>
    /// Function to set index of a pointer.
    /// </summary>
    /// <param name="nr">number of pointer to set</param>
    /// <param name="index">index to set pointer to</param>
    void setIndex(int nr, int index);
    /// <summary>
    /// Function to get length of section from pointer.
    /// </summary>
    /// <param name="nr">number of pointer to get from</param>
    /// <returns>length of section</returns>
    int getLength(int nr);
    /// <summary>
    /// Function to get length of section from pointer.
    /// </summary>
    /// <param name="nr">number of pointer to set</param>
    /// <param name="length">length of section</param>
    void setLength(int nr, int length);
    /// <summary>
    /// Function to get all values of a pointer.
    /// </summary>
    /// <param name="nr">number of pointer to read from</param>
    /// <param name="id">id of section</param>
    /// <param name="length">length of section</param>
    /// <param name="index">index of section</param>
    void getPointer(int nr,ushort& id, int& length,int& index);
    /// <summary>
    /// Function to set all values of a pointer
    /// </summary>
    /// <param name="nr">number of pointer to set</param>
    /// <param name="id">id of section</param>
    /// <param name="length">length of section</param>
    /// <param name="index">index of section</param>
    void setPointer(int nr, ushort id, int length, int index);
protected:		
    int _Write(byte[] buffer, int offset);
    void _Empty();
    int _getLength();
private:
    // Defined in SCP.
    static byte[] _Reserved;
    static ushort _SectionID;
    static int _NrMandatory;

    class SCPPointer;
    // Part of the stored Data Structure.
    SCPPointer[] _MandatoryPointers;
    SCPPointer[] _OptionalPointers;
};
}

#endif  /*#ifndef _SCPSECTION0_H_*/
