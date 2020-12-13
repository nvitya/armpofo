// exprcalc.h
//
// Expression calculator

#ifndef EXPRCALC_H_
#define EXPRCALC_H_

#include "strparse.h"

#define CALC_VAR_NAME_LENGTH    16
#define CALC_VAR_COUNT          32
#define CALC_ERR_MSG_LEN       128

class TCalcValue
{
public:
	double        floatvalue = 0.0l;

	void Assign(TCalcValue * avar);

	void AssignU32(unsigned auint);   // for hexadecimal / binary operations
	void AssignFloat(double afloat);
};

class TCalcVar
{
public:
	char          name[CALC_VAR_NAME_LENGTH];
	TCalcValue    value;
	unsigned      use_count;
};

class TExprCalc
{
public:
	TStrParseObj  sp;

	const char *  expression = nullptr;
	unsigned      exprlen = 0;

	bool          error = false;
	char          errormsg[CALC_ERR_MSG_LEN];

	TCalcValue    ans; // previous result
	TCalcVar      varlist[CALC_VAR_COUNT];

	bool          store_result = false;
	char          store_varname[CALC_VAR_NAME_LENGTH];

	virtual ~TExprCalc() { }  // not dynamically allocated

	bool Evaluate(const char * aexpr, unsigned aexprlen, TCalcValue * rresult);

public:
	bool ProcessIdentifier(TCalcValue * rresult);

	bool CalcNumber(TCalcValue * rresult);

	bool CalcFactor(TCalcValue * rresult);         // operators: ^
	bool CalcTerm(TCalcValue * rresult);           // operators: * /
	bool CalcExpression(TCalcValue * rresult);     // operators: + -

	bool ReadFunctionArgument(TCalcValue * rresult);  // only for single argument functions

	bool SetError(const char * aerrormsg);// { errormsg = aerrormsg; error = true;  return false; }

	void SetVar(const char * avarname, TCalcValue * avalue);

};

#endif /* EXPRCALC_H_ */
