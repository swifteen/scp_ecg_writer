#include"Drug.h"
namespace ECGConversion.ECGDemographics
{
/// <summary>
/// A class to store a drug (is very close to the implementation of both SCP and UNIPRO).
/// </summary>


Drug::Drug()
{

}

Drug::Drug(byte drugClass,byte classCode,string textDesciption)
{
    DrugClass = drugClass;
    ClassCode = classCode;
    TextDesciption = textDesciption;

}

}
