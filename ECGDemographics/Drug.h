#ifndef _DRUG_H_
#define _DRUG_H_
#include "ScpGlobal.h"


namespace ECGConversion
{

namespace ECGDemographics
{

/// <summary>
/// A class to store a drug (is very close to the implementation of both SCP and UNIPRO).
/// </summary>
class Drug
{
public:
    Drug();
    Drug(uchar drugClass,uchar ClassCode,std::string TextDesciption);
public:
    uchar DrugClass;
    uchar ClassCode;
    std::string TextDesciption;
};
}
}
#endif  /*#ifndef _DRUG_H_*/

