#include "Drug.h"
namespace ECGConversion
{

namespace ECGDemographics
{
/// <summary>
/// A class to store a drug (is very close to the implementation of both SCP and UNIPRO).
/// </summary>


Drug::Drug()
{

}

Drug::Drug(uchar drugClass,uchar classCode,std::string textDesciption)
{
    DrugClass = drugClass;
    ClassCode = classCode;
    TextDesciption = textDesciption;

}

}

}
