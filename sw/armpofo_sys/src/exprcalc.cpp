// exprcalc.cpp
//
// Expression Calculator

#include "exprcalc.h"
#include "math.h"
#include "string.h"
#include "stdio.h"

void TCalcValue::Assign(TCalcValue * avar)
{
	floatvalue = avar->floatvalue;
}

void TCalcValue::AssignU32(unsigned auint)
{
	floatvalue = auint;
}

void TCalcValue::AssignFloat(double afloat)
{
	floatvalue = afloat;
}


//-----------------------------------------------------------------------

bool TExprCalc::Evaluate(const char * aexpr, unsigned aexprlen, TCalcValue * rresult)
{
	expression = aexpr;
	exprlen = aexprlen;

	errormsg[0] = 0;
	error = false;
	store_result = false;

	sp.Init((char *)expression, exprlen);
	sp.SkipSpaces();

	// check for result storage: "X = 123"
	if (sp.ReadToChar('='))
	{
		sp.CheckSymbol("="); // skip
		sp.SkipSpaces();
		char * exprbegin = sp.readptr;  // save the begin of the expression

		// now parse the variable name
		sp.Init((char *)expression, exprlen);
		sp.SkipSpaces();
		if (sp.ReadIdentifier())
		{
			if (sp.prevlen >= CALC_VAR_NAME_LENGTH)
			{
				return SetError("Variable name too long");
			}
			else
			{
			  store_result = true;
			  strncpy(&store_varname[0], sp.prevptr, sp.prevlen);
			  sp.readptr = exprbegin;
			}
		}
		else
		{
			return SetError("Identifier before \"=\" is missing");
		}
	}
	else
	{
		sp.readptr = sp.bufstart;
	}

	if (!CalcExpression(rresult))
	{
		return false;
	}

	ans.Assign(rresult);

	if (store_result)
	{
		SetVar(&store_varname[0], rresult);
	}

	return true;
}

bool TExprCalc::ReadFunctionArgument(TCalcValue * rresult)  // expects '(' expr. ')'
{
	sp.SkipSpaces();
	if (!sp.CheckSymbol("("))  return SetError("\"(\" is missing");
	sp.SkipSpaces();
	if (!CalcExpression(rresult))
	{
		return false;
	}
	sp.SkipSpaces();
	if (!sp.CheckSymbol(")"))  return SetError("\")\" is missing");
	sp.SkipSpaces();
  return true;
}

bool TExprCalc::ProcessIdentifier(TCalcValue * rresult)  // input in sp.prevptr + sp.prevlen
{
	TCalcValue  arg1;

	if (sp.UCComparePrev("SQRT"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( sqrt(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("EXP"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( exp(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("LN") || sp.UCComparePrev("LOG"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( log(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("SIN"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( sin(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("ASIN"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( asin(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("COS"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( cos(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("ACOS"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( acos(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("TAN"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( tan(arg1.floatvalue) );
	}
	else if (sp.UCComparePrev("ATAN"))
	{
		if (!ReadFunctionArgument(&arg1))  return false;
		rresult->AssignFloat( tan(arg1.floatvalue) );
	}

	// constants
	else if (sp.UCComparePrev("PI"))
	{
		rresult->AssignFloat( M_PI );
	}
	else if (sp.UCComparePrev("ANS"))  // previous result
	{
		rresult->Assign(&ans);
	}
	else
	{
		// search trough the variables
		unsigned n;
		for (n = 0; n < CALC_VAR_COUNT; ++n)
		{
			TCalcVar * pvar = &varlist[n];
			if (pvar->use_count && sp.ComparePrev(pvar->name))
			{
				++(pvar->use_count);
				rresult->Assign( &pvar->value );
				return true;
			}
		}

		// else not found.

		char idnamestr[CALC_VAR_NAME_LENGTH];
		strncpy(&idnamestr[0], sp.prevptr, sizeof(idnamestr));
		sprintf(&errormsg[0], "Unknown identifier \"%s\"", &idnamestr[0]);
		error = true;
		return false;
	}

	return true;
}

bool TExprCalc::CalcNumber(TCalcValue * rresult)
{
	TCalcValue  var1;
	bool      negate = false;

	sp.SkipWhite();

	if (sp.CheckSymbol("("))
	{
		if (!CalcExpression(&var1))  return false;

		if (!sp.CheckSymbol(")"))
		{
			return SetError("\")\" is missing");
		}
		rresult->Assign(&var1);
		return true;
	}

	// single constant or variable

	if (sp.CheckSymbol("-"))
	{
		negate = true;
	}

	sp.SkipSpaces();

	if (sp.readptr >= sp.bufend)
	{
		return SetError("Number is missing");
	}

	char c = *sp.readptr;
	if (((c >= '0') && (c <= '9')) || ('.' == c))
	{
		// number
		if (sp.CheckSymbol("0x"))
		{
			// hex value
			if (!sp.ReadHexNumbers())
			{
				return SetError("Invalid Hex Number");
			}
			var1.AssignU32(sp.PrevHexToInt());
		}
		else
		{
			var1.AssignFloat(sp.ParseFloatNumber());
		}
	}
	else if (((c >= 'a') && (c <= 'z')) || ((c >= 'a') && (c <= 'z')) || ('_' == c))
	{
		// identifier (variable or function)
		sp.ReadAlphaNum();
		if (!ProcessIdentifier(&var1))  return false;
	}
	else
	{
		sprintf(&errormsg[0], "Invalid char: \"%c\"", c);
		error = true;
		return false;
	}

	if (negate)
	{
		var1.AssignFloat( -var1.floatvalue );
	}

	rresult->Assign(&var1);

	return true;
}

bool TExprCalc::CalcFactor(TCalcValue * rresult)
{
	TCalcValue var1;
	TCalcValue var2;

	if (!CalcNumber(&var1))
	{
		return false;
	}

	while (!error)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("^"))
		{
			if (!CalcNumber(&var2))  return false;
			var1.AssignFloat( pow(var1.floatvalue, var2.floatvalue) );
		}
		else
		{
			break;
		}
	}

	if (!error)
	{
		rresult->Assign(&var1);
	}

	return true;
}

bool TExprCalc::CalcTerm(TCalcValue * rresult)
{
	TCalcValue var1;
	TCalcValue var2;
	if (!CalcFactor(&var1))
	{
		return false;
	}

	while (!error)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("*"))
		{
			if (!CalcFactor(&var2))  return false;
			var1.AssignFloat( var1.floatvalue * var2.floatvalue );
		}
		else if (sp.CheckSymbol("/"))
		{
			if (!CalcFactor(&var2))  return false;
			if (var2.floatvalue == 0.0)
			{
				return SetError("Division by zero");
			}
			else
			{
				var1.AssignFloat( var1.floatvalue / var2.floatvalue );
			}
		}
		else
		{
			break;
		}
	}

	if (!error)
	{
		rresult->Assign(&var1);
	}

	return true;
}

bool TExprCalc::CalcExpression(TCalcValue * rresult)
{
	TCalcValue var1;
	TCalcValue var2;
	if (!CalcTerm(&var1))
	{
		return false;
	}

	while (!error)
	{
		sp.SkipWhite();
		if (sp.CheckSymbol("+"))
		{
			if (!CalcTerm(&var2))  return false;
			var1.AssignFloat( var1.floatvalue + var2.floatvalue );
		}
		else if (sp.CheckSymbol("-"))
		{
			if (!CalcTerm(&var2))  return false;
			var1.AssignFloat( var1.floatvalue - var2.floatvalue );
		}
		else
		{
			break;
		}
	}

	if (!error)
	{
		rresult->Assign(&var1);
	}

	return true;
}

bool TExprCalc::SetError(const char * aerrormsg)
{
	strncpy(&errormsg[0], aerrormsg, sizeof(errormsg));
	error = true;
	return false;
}

void TExprCalc::SetVar(const char * avarname, TCalcValue * avalue)
{
	unsigned n;
	TCalcVar * pvar;
	TCalcVar * pvictim = nullptr;

	for (n = 0; n < CALC_VAR_COUNT; ++n)
	{
		pvar = &varlist[n];
		if (pvar->use_count == 0) // empty, when we are so far then this is it
		{
			strncpy(&pvar->name[0], avarname, CALC_VAR_NAME_LENGTH);
			pvar->value.Assign(avalue);
			pvar->use_count = 1;
			return;
		}

		if (strncmp(pvar->name, avarname, CALC_VAR_NAME_LENGTH) == 0)
		{
			++(pvar->use_count);
			pvar->value.Assign(avalue);
			return;
		}

		if (!pvictim || (pvictim->use_count > pvar->use_count))
		{
			pvictim = pvar;
		}
	}

	if (pvictim)  // this should be set, when we are so far
	{
		strncpy(&pvar->name[0], avarname, CALC_VAR_NAME_LENGTH);
		pvar->value.Assign(avalue);
		pvar->use_count = 1;
	}
}
