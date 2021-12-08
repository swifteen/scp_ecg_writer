#ifndef _ZQ_STATEMENTS_H_
#define _ZQ_STATEMENTS_H_


namespace ECGConversion.ECGDiagnostic
{
/// <summary>
/// Class containing diagnostic statements.
/// </summary>
class Statements
{

public:
    Statements();
public:
    bool confirmed;
    bool interpreted;
    DateTime time;
    string[] statement;
};
}
