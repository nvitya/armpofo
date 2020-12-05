// exprcalc.h
//
// Expression calculator

#ifndef EXPRCALC_H_
#define EXPRCALC_H_

#include "strparse.h"

// Error Codes
#define EEXPR_OK                        0
#define EEXPR_DIV_BY_ZERO               1
#define EEXPR_OPENING_BRACKET_MISSING   2
#define EEXPR_CLOSING_BRACKET_MISSING   3
#define EEXPR_INVALID_DEC_NUMBER        4
#define EEXPR_INVALID_HEX_NUMBER        5
#define EEXPR_INVALID_CHAR              6
#define EEXPR_UNKNOWN_IDENTIFIER        7

class TCalcVar
{
public:
	double        floatvalue = 0.0l;

	int Assign(TCalcVar * avar);

	int AssignU32(unsigned auint);   // for hexadecimal / binary operations
	int AssignFloat(double afloat);

	int Add(TCalcVar * avar);
	int Sub(TCalcVar * avar);
	int Mul(TCalcVar * avar);
	int Div(TCalcVar * avar);
	int Pow(TCalcVar * avar);

	int Negate();
};

class TExprCalc
{
public:
	TStrParseObj  sp;

	const char *  expression = nullptr;
	unsigned      exprlen = 0;

	int           error = 0;

	virtual ~TExprCalc() { }  // not dynamically allocated

	int Evaluate(const char * aexpr, unsigned aexprlen, TCalcVar * rresult);

public:
	int ProcessIdentifier(TCalcVar * rresult);

	int CalcNumber(TCalcVar * rresult);

	int CalcFactor(TCalcVar * rresult);         // operators: ^
	int CalcTerm(TCalcVar * rresult);           // operators: * /
	int CalcExpression(TCalcVar * rresult);     // operators: + -

	int ReadFunctionArgument(TCalcVar * rresult);  // only for single argument functions

};

#endif /* EXPRCALC_H_ */
