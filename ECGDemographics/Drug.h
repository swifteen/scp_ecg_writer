#ifndef _DRUG_H_
#define _DRUG_H_


namespace ECGConversion.ECGDemographics
{
/// <summary>
/// A class to store a drug (is very close to the implementation of both SCP and UNIPRO).
/// </summary>
class Drug
{
public:
    Drug();
    Drug(byte drugClass,byte ClassCode,string TextDesciption);
public:
    byte DrugClass;
    byte ClassCode;
    string TextDesciption;
};
}
